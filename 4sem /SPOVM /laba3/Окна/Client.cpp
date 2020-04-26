#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main()
{
	HANDLE work = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("work"));
	HANDLE close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, TEXT("close"));
	HANDLE WINAPI hFileProjection = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("FileProjection"));
	if (hFileProjection == NULL)
	{
		cout << "OpenFileMapping error!" << endl << "Error: " << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	//сопоставим объект файловой проекции с адресным пространством
	LPVOID memoryMap;
	memoryMap = MapViewOfFile(hFileProjection, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (memoryMap == NULL)
	{
		cout << "MapViewOfFile error!" << endl << "Error: " << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	char* strRepresent = (char*)memoryMap;

	while (true)
	{
		WaitForSingleObject(work, INFINITY);
		if (WaitForSingleObject(close, 100) == WAIT_OBJECT_0)
		{
			UnmapViewOfFile(memoryMap);
			CloseHandle(hFileProjection);

			CloseHandle(work);
			CloseHandle(close);

			return 0;
		}

		cout << "Client proccess:" << endl << "Message: " << strRepresent << endl << endl;

		Sleep(1000);
		ReleaseSemaphore(work, 1, NULL);
	}
}