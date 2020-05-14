#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

int tCount = 0;

DWORD WINAPI ThreadFunction(HANDLE mut)
{
	HANDLE hMutex = mut;
	string str = "Thread_" + to_string(++tCount);

	while (true)
	{
		WaitForSingleObject(hMutex, INFINITY);
		
		for (int i = 0; i < str.length(); ++i)
		{
			cout << str[i];
		}
		cout << endl;

		ReleaseMutex(hMutex);

		Sleep(1000);
	}

	return 0;
}

int main()
{
	char c;
	vector<HANDLE> hThreads;

	HANDLE hMutex = CreateMutex(NULL, //�������� ������
		FALSE, //���������� �� ����� ����� �������������
		TEXT("hMutex")); // ��� ��������
	if (hMutex == NULL)
	{
		cout << "Create mutex error!" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Add new thread: +" << endl;
	cout << "Delete last thread: -" << endl;
	cout << "Exit the programm: q" << endl;

	while (true)
	{
		c = _getch();
		switch (c)
		{
		case '+':
		{
			HANDLE newThread = CreateThread(NULL, //�������� ������
				0, //��������� ������ ����� � ������
				&ThreadFunction, //��������� �� �������, ������� ����� ����������� �������
				hMutex, //��������� �� ���������, ������� 
				0, //����������� ����-����� ��������� ����� ����� ��������
				NULL); //��������� �� ����������, ������� �������� ������������� ������
			if (newThread == NULL)
			{
				cout << "Create thread error!" << endl;
				exit(EXIT_FAILURE);
			}

			hThreads.emplace_back(newThread);

			break;
		}
		case '-':
		{
			if (hThreads.empty())
			{
				cout << "Vector is empty!" << endl;
			}
			else
			{
				--tCount;
				if (TerminateThread(hThreads.back(), NO_ERROR) == 0)
				{
					cout << "TerminateThread error!" << endl;
					cout << "Error: " << GetLastError() << endl;
					exit(EXIT_FAILURE);
				}
				hThreads.pop_back();
			}

			break;
		}
		case 'q':
		{
			while (!hThreads.empty())
			{
				if (TerminateThread(hThreads.back(), NO_ERROR) == 0)
				{
					cout << "TerminateThread error!" << endl;
					cout << "Error: " << GetLastError() << endl;
					exit(EXIT_FAILURE);
				}
				hThreads.pop_back();
			}

			CloseHandle(hMutex);
			return 0;
		}
		default:
		{
			cout << "Input error! Try again" << endl;
			break;
		}
		}
	}
}