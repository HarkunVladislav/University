#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUF_SIZE 512

using namespace std;

const char *read_sem_name = "read_semaphore";
const char *write_sem_name = "write_semaphore";
const char *child_name = "/home/vladislav/SPOVM/laba3/client";

sem_t* open_sem(const char *sem_name, unsigned int value)
{
	sem_t *sem = sem_open(sem_name, O_CREAT, S_IRWXO | S_IRWXU, value);
	if(sem == SEM_FAILED)
	{
		cout << "Can't create a semaphore!" << endl;
		return NULL;
	}

	return sem;
}

int main()
{
	sem_t *read_sem = open_sem(read_sem_name, 0);
	sem_t *write_sem = open_sem(write_sem_name, 1);

	int pipefd[2]; //массив для хранения дискрипторов файла, для чтения и записи

	//создание канала
	if(pipe(pipefd) == -1)
	{
		cout << "Can't make a pipe!" << endl;
		exit(EXIT_FAILURE);
	}

	char str[BUF_SIZE];

	char **args = new char*[2];
	args[0] = new char[strlen(child_name) + 1];
	strncpy(args[0], child_name, strlen(child_name) + 1);
	args[1] = NULL;

	pid_t pid = fork();
	if(pid == -1)
	{
		cout << "Can't create a child process!" << endl;

		close(pipefd[1]);
		close(pipefd[0]);

		sem_close(read_sem);
		sem_close(write_sem);
		sem_unlink(read_sem_name);
		sem_unlink(write_sem_name);

		exit(EXIT_FAILURE);
	}
	else
	{
		if(pid == 0)
		{
			close(pipefd[1]);
			if(dup2(pipefd[0], 0 == -1))
			{
				cout << "Failed to dup read end!" << endl;
				exit(EXIT_FAILURE);
			}
			close(pipefd[0]);

			if(execvp(child_name, args) == -1)
			{
				cout << "Execvp error!" << endl;
				exit(EXIT_FAILURE);
			}
		}
	}

	close(pipefd[0]);

	while(true)
	{
		sem_wait(write_sem);

		fgets(str, BUF_SIZE, stdin);
		if(*str == '\0' || *str == '\n' || *str == '\r')
		{
			kill(pid, SIGTERM);
			break;
		}

		write(pipefd[1], str, BUF_SIZE);
		sem_post(read_sem);
	}

	close(pipefd[1]);

	sem_close(read_sem);
	sem_close(write_sem);
	sem_unlink(read_sem_name);
	sem_unlink(write_sem_name);

	delete [] args[0];
	delete args;

	exit(EXIT_SUCCESS); 
}