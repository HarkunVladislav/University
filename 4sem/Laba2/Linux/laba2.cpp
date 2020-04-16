#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;

vector<pid_t> pidVector;
bool childOutput = false;
bool childExit = false;
int num = 0;

struct sigaction p_action, c_action, c_exit;

char _getch();
void childProcess(int pCount);

void output(int signum) 
{
	num++; 
	if(num >= pidVector.size())
 		num=0; 
	kill(pidVector[num], SIGUSR1); 
}

void set_child(int signum)
{
    childOutput = true;
}

void child_exit(int signum)
{
    childExit = true;
}

int main(int argc, char* argv[])
{

    p_action.sa_handler = output;
    p_action.sa_flags = 0;
    sigaction(SIGUSR1, &p_action, NULL);

    int pCount = 0;

    cout << "Create new process: '+'" << endl;
    cout << "Delete last process: '-'" << endl;
    cout << "Exit the programm: 'q'" << endl;

    while (true)
    {
        switch (_getch())
        {
        	case '+': 
        	{
            	pCount++;
            	pid_t pid = fork();
    			switch (pid)
    			{
    			case -1: 
    				cout << "Error!\n"; 
    				break;
    			case 0: 
    				childProcess(pCount);
    				break;
    			default: 
    				pidVector.push_back(pid); 
    				break;
    			}

            	if (pidVector.size() == 1)
           	 	{
               	 	usleep(150);
                	kill(pidVector.back(), SIGUSR1);
           
            	}
        	}
        	break;

       
 			case '-':
 				if (pidVector.size())
 				{
        			kill(pidVector.back(), SIGUSR2);
       				waitpid(pidVector.back(), NULL, 0);
     				pidVector.pop_back();
   				}
   				else 
    			{
        			cout << "There are no processes!\n";
    			}

    		break;
        
			case 'q':  
    			while (pidVector.size())
    			kill(pidVector.back(), SIGUSR2);
    			waitpid(pidVector.back(), NULL, 0);
    			pidVector.pop_back();
    			exit(0);
    			break;
    
   		}
	}
}

void childProcess(int number) 
{
    c_exit.sa_handler = child_exit;
    c_exit.sa_flags = 0;
    sigaction(SIGUSR2, &c_exit, NULL);

    c_action.sa_handler = set_child;
    c_action.sa_flags = 0;
    sigaction(SIGUSR1, &c_action, NULL);

    char pInf[20];
    sprintf(pInf, "Process_%d ", number);

    while (true) {

        if (childOutput) {
            usleep(150000);
            childOutput = false;

            for (int i = 0; i < strlen(pInf); i++)
            {
                cout << pInf[i];
                usleep(10000);
            }
            cout << "\n";

            kill(getppid(), SIGUSR1);
        }
        if (childExit) {
            exit(0);
        }

    }
}

char _getch()
{
    struct termios oldAttr, newAttr;
    tcgetattr(STDIN_FILENO, &oldAttr);
    newAttr = oldAttr;
    newAttr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
    char symbol;
    symbol = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);
    return symbol;
}
