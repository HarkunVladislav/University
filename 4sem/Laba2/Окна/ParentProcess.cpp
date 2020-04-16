#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <vector>
#include <cstdlib>

using namespace std;

HANDLE CreateNewProcess(int pNumber);

int main(int argc, char** argv)
{
	cout << "Create new process: +" << endl;
	cout << "Kill last process: -" << endl;
	cout << "Exit the programm: q" << endl;

	int pCount = 0;
	char c;

	HANDLE Event = CreateEvent(NULL, false, true, LPWSTR("Event"));
	vector<HANDLE> pVector;

	while (true)
	{
		do
		{
			c = _getch();
		} while (c != '+' && c != '-' && c != 'q');

		switch (c)
		{
		case '+':
			pVector.push_back(CreateNewProcess(pCount++));
			if (pVector.size() == 1)
				SetEvent(Event);
			break;
		case '-':
			if (pVector.empty())
			{
				cout << "Vector is empty!" << endl;
			}
			else
			{
				--pCount;
				TerminateProcess(pVector.back(), EXIT_SUCCESS);
				CloseHandle(pVector.back());
				pVector.pop_back();
			}
			break;
		case 'q':
			while (!pVector.empty())
			{
				TerminateProcess(pVector.back(), EXIT_SUCCESS);
				CloseHandle(pVector.back());
				pVector.pop_back();
			}

			return 0;
		}

		Sleep(500);
	}
}

HANDLE CreateNewProcess(int pNumber)
{
	wstring CommandArgs = L"Process_" + to_wstring(pNumber) + L"\n" + L"\0";
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;

	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

	TCHAR cProcess[] = TEXT("D:\\4sem\\СПОВМ\\laba2\\Debug\\ChildProcess.exe");

	if (!CreateProcess(cProcess, (const LPWSTR)CommandArgs.c_str(), NULL, NULL, FALSE,	NULL, NULL, NULL, &StartupInfo, &ProcessInfo))
	{
		cout << "Create process error:" << endl;
		cout << GetLastError() << endl;

		//Sleep(500);
		exit(EXIT_FAILURE);
	}

	return ProcessInfo.hProcess;
}

