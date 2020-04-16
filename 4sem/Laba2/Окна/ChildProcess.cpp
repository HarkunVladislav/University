#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <vector>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, LPWSTR("Event"));

	while (true)
	{
		WaitForSingleObject(hEvent, INFINITY);
		for (int i = 0; i < strlen(argv[0]); i++)
			cout << argv[0][i];

		SetEvent(hEvent);
		Sleep(3000);
	}
	
	return 0;
}