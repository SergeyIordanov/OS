// OS_lab_6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "myHeader.h"

int _tmain()
{
	TCHAR cl1NoThreads[] = _T("Task1.exe 0");
	TCHAR cl1Threads[] = _T("Task1.exe 1");
	TCHAR cl2[] = _T("Task2.exe");
	TCHAR cl34[] = _T("Task3(4).exe");
	TCHAR cl5[] = _T("Task5.exe");

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	DWORD code;
	BOOL b;
	char c = -1;
	
	while (c != '0')
	{
		if (c == -1 || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6')
		{
			printf("0. Close the program\n");
			printf("1. Start 10 processes (NO threads)\n");
			printf("2. Start 10 processes (10 threads)\n");
			printf("3. Count max number of threads\n");
			printf("4. Readers & writers\n");
			printf("5. Philosophers' dinner\n");
			printf("6. Readers & writers with timer\n");
		}
		c = getchar();
		switch (c)
		{
		case '1':
			b = CreateUnsuspendedProcess(cl1NoThreads, 0, &si, &pi);

			if (b) {
				WaitForSingleObject(pi.hProcess, INFINITE);

				GetExitCodeProcess(pi.hProcess, &code);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (code != 0) {
					cout << "The process completed with error!\nPress any key to continue.." << endl;
					getchar();
				}
				cout << "\nDone!\n" << endl;
			}
			break;
		case '2':
			b = CreateUnsuspendedProcess(cl1Threads, 0, &si, &pi);

			if (b) {
				WaitForSingleObject(pi.hProcess, INFINITE);

				GetExitCodeProcess(pi.hProcess, &code);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (code != 0) {
					cout << "The process completed with error!\nPress any key to continue.." << endl;
					getchar();
				}
				cout << "\nDone!\n" << endl;
			}
			break;
		case '3':
			b = CreateUnsuspendedProcess(cl2, 0, &si, &pi);

			if (b) {
				WaitForSingleObject(pi.hProcess, INFINITE);

				GetExitCodeProcess(pi.hProcess, &code);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (code != 0) {
					cout << "The process completed with error!\nPress any key to continue.." << endl;
					getchar();
				}
				cout << "\nDone!\n" << endl;
			}
			break;
		case '4':
			b = CreateUnsuspendedProcess(cl34, 0, &si, &pi);

			if (b) {
				WaitForSingleObject(pi.hProcess, INFINITE);

				GetExitCodeProcess(pi.hProcess, &code);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (code != 0) {
					cout << "The process completed with error!\nPress any key to continue.." << endl;
					getchar();
				}
				cout << "\nDone!\n" << endl;
			}
			break;
		case '5':
			b = CreateUnsuspendedProcess(cl5, 0, &si, &pi);

			if (b) {
				WaitForSingleObject(pi.hProcess, INFINITE);

				GetExitCodeProcess(pi.hProcess, &code);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				if (code != 0) {
					cout << "The process completed with error!\nPress any key to continue.." << endl;
					getchar();
				}
				cout << "\nDone!\n" << endl;
			}
			break;
		case '6':
			HANDLE hTimer = NULL;
			LARGE_INTEGER liDueTime;
			//liDueTime.QuadPart = -10000000LL;
			liDueTime.QuadPart = -50000000LL;
			for (int i = 0; i < 4; i++) {
				b = CreateUnsuspendedProcess(cl34, 0, &si, &pi);
				if (b) {
					WaitForSingleObject(pi.hProcess, INFINITE);

					GetExitCodeProcess(pi.hProcess, &code);

					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					if (code != 0) {
						cout << "The process completed with error!\nPress any key to continue.." << endl;
						getchar();
					}
					
					printf("Next process in 5 seconds...\n\n");
					hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
					if (NULL == hTimer)
					{
						_tprintf(_T("Error"));
						return -1;
					}

					if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
					{
						_tprintf(_T("Error"));
						return -1;
					}

					if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
					{
						_tprintf(_T("Error"));
					}
					else
					{
						printf("\nTimer ends.\n\n");
					}
				}
			}
			cout << "\nDone!\n" << endl;
			break;
		}
	}
    return 0;
}

