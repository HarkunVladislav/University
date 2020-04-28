#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUF_SIZE 512

using namespace std;

const char *read_sem_name = "read_semaphore";
const char *write_sem_name = "write_semaphore";

sem_t *read_sem = NULL;
sem_t *write_sem = NULL;

void sig_handler(int signum)
{
	sem_close(read_sem);
	sem_close(write_sem);
	exit(EXIT_SUCCESS);
}

sem_t *open_sem(const char *sem_name)
{
	sem_t *sem = sem_open(sem_name, 0);
	if(sem == SEM_FAILED)
	{
		cout << "Can't open a semaphore!" << endl;
		return NULL;
	}

	return sem;
}

int main()
{
	read_sem = open_sem(read_sem_name);
	write_sem = open_sem(write_sem_name);

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	act.sa_handler = sig_handler;

	if(sigaction(SIGTERM, &act, NULL) == -1)
	{
		cout << "Sigaction failed!" << endl;
		exit(EXIT_FAILURE);
	}

	char str[BUF_SIZE];

	while(true)
	{
		sem_wait(read_sem);

		read(0, str, BUF_SIZE);
		cout << str << endl;

		sem_post(write_sem);
	}

	exit(EXIT_SUCCESS);
}