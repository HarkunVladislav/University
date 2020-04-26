#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>

using namespace std;

int main()
{
	char c;
	int maxSize = 80;
	DWORD maxSizeHigh = 0, maxSizeLow = 80;

	HANDLE work = CreateSemaphore(NULL, 0, 1, TEXT("work"));
	HANDLE close = CreateSemaphore(NULL, 0, 1, TEXT("close"));

	//создаем проекцию файла
	HANDLE WINAPI hFileProjection = CreateFileMapping(INVALID_HANDLE_VALUE, //передается, тк необходимо только выделить общуб область памяти
		NULL, //атрибуты защиты
		PAGE_READWRITE, //дотуп только для чтения, копирования при записи или чтения и записи
		maxSizeHigh, //старшее слово размера выделяемой памяти
		maxSizeLow, //младшее слово размера выделяемой памяти
		TEXT("FileProjection")); //имя объекта
	if (hFileProjection == NULL)
	{
		cout << "CreateFileMapping error!" << endl << "Error: " << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	STARTUPINFO StartUpInfo;
	ZeroMemory(&StartUpInfo, sizeof(STARTUPINFO));
	StartUpInfo.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION ProcInfo;

	TCHAR path[] = TEXT("D:\\4sem\\СПОВМ\\laba3\\Debug\\Client.exe");
	TCHAR CommadLine[] = TEXT("Client");
	if (!CreateProcess(path,
		CommadLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&StartUpInfo,
		&ProcInfo))
	{
		cout << "Create process failed!" << endl << "Error: " << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	LPVOID memoryMap; //указатель на область памяти
	memoryMap = MapViewOfFile(hFileProjection, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (memoryMap == NULL)
	{
		cout << "MapViewOfFile error!" << endl << "Error: " << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	char* strRepresent = (char*)memoryMap;
	int curPos = 0;
	bool input = true;
	string strBuf;
	strBuf.resize(maxSize, '\0');

	while (true)
	{
		system("CLS");

		if (input)
		{
			curPos = 0;

			cout << "Server Process:" << endl << "Enter the string: ";
			cin >> strBuf;
			input = false;
		}

		//string tempBuf;
		//int newLength = 0;

		//tempBuf.append(strBuf, 0, maxSize - 1);
		//curPos = tempBuf.length();
		strcpy(strRepresent, const_cast<char*>(strBuf.c_str()));
		//strRepresent[tempBuf.length()] = '\0';
		//tempBuf.clear();

		//newLength = strBuf.length() - curPos;
		//if (newLength > 0)
		//	tempBuf.append(strBuf, curPos, newLength);

		strBuf.clear();// = tempBuf;

		ReleaseSemaphore(work, 1, NULL);
		WaitForSingleObject(work, INFINITY);

		if (strBuf.empty())
		{
			input = true;

			cout << "Press '0' to exit the program";
			c = _getche();
			if (c == '0')
			{
				ReleaseSemaphore(close, 1, NULL);
				ReleaseSemaphore(work, 1, NULL);
				WaitForSingleObject(ProcInfo.hProcess, INFINITY);
				
				//закрываем отображение файла
				UnmapViewOfFile(memoryMap);
				CloseHandle(hFileProjection);

				CloseHandle(ProcInfo.hThread);
				CloseHandle(ProcInfo.hProcess);
				CloseHandle(close);
				CloseHandle(work);
				
				return 0;
			}

			strBuf.clear();
			cin.clear();
		}
	}
}