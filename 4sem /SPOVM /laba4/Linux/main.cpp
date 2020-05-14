#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <cstring>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

vector<pthread_t> tVector;
int tCount = 0;

int _getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void* threadFunction(void*)
{
	string str = "Thread_" + to_string(tCount);

	while(true)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		pthread_mutex_lock(&mutex);

		for(int i = 0; i < str.length(); ++i)
		{
			cout << str[i];
			//usleep(100000);
		}
		cout << "\n";

		pthread_mutex_unlock(&mutex);

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		sleep(1);
	}

	pthread_exit(NULL);
}

void create_thread()
{
	pthread_t thread;

	pthread_create(&thread, NULL, threadFunction, NULL);
	tVector.emplace_back(thread);
}

int terminate_thread()
{
	pthread_t thread;

	if(tVector.empty())
	{
		return 0;
	}
	else
	{
		--tCount;
		thread = tVector.back();
		tVector.pop_back();
	}

	pthread_cancel(thread);

	return 1;
}

int main()
{
	char c;

	while(true)
	{
		c = _getch();

		switch(c)
		{
			case '+':
				++tCount;
				create_thread();
				break;

			case '-':
				if(!terminate_thread())
				{
					cout << "Vector is empty" << endl;
				}
				break;

			case 'q':
				while(terminate_thread());
				pthread_mutex_destroy(&mutex);
				exit(EXIT_SUCCESS);

			default:
				cout << "Incorrect input! Try again" << endl;
				break;
		}		
	}
}