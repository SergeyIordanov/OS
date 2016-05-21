// OS_lab_5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Macroses.h"

#pragma warning( disable : 4996 )
int _tmain()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	DWORD code;
	BOOL b;

	SYSTEMTIME st;
	FILETIME ft;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	TCHAR cl1[] = _T("NotepadRunner.exe");
	TCHAR cl2[MAX_PATH];
	_swprintf(cl2, L"FilesInfo.exe %u %u", ft.dwLowDateTime, ft.dwHighDateTime);
	TCHAR cl3[] = _T("ProcessesInfo.exe");

	cout << "----- Program 1 (Notepad) -----" << endl;
	printf("Press any key..\n");
	getchar();

	b = CreateUnsuspendedProcess(cl1, 0, &si, &pi);

	if (b) {
		WaitForSingleObject(pi.hProcess, INFINITE);

		GetExitCodeProcess(pi.hProcess, &code);
		
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (code != 0) {
			cout << "The first programm completed with error!\nPress any key to exit.." << endl;
			getchar();
			return 0;
		}
		cout << "The first programm has been completed!\n" << endl;
	}

	cout << "----- Program 2 (Files info) -----" << endl;
	printf("Press any key..\n");
	getchar();

	b = CreateProcess(0, cl2, 0, 0, 0, BELOW_NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);
	
	if (b) {	

		WaitForSingleObject(pi.hProcess, INFINITE);

		GetExitCodeProcess(pi.hProcess, &code);

		if (code != 0) {
			cout << "The second programm completed with error!\nPress any key to exit.." << endl;
			getchar();
			return 0;
		}

		cout << "The second programm has been completed!\n" << endl;

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	
	cout << "----- Program 3 (Processes info) -----" << endl;
	printf("Press any key..\n");
	getchar();

	b = CreateUnsuspendedProcess(cl3, 0, &si, &pi);

	if (b) {

		WaitForSingleObject(pi.hProcess, INFINITE);

		GetExitCodeProcess(pi.hProcess, &code);

		if (code != 0) {
			cout << "The third programm completed with error!\nPress any key to exit.." << endl;
			getchar();
			return 0;
		}

		cout << "\nThe third programm has been completed!\n" << endl;

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	getchar();
    return 0;
}

