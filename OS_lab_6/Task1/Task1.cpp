// Task1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "../OS_lab_6/myHeader.h"
#include "ThreadFunc.h"

#pragma warning( disable : 4996 )
int _tmain(int argc, _TCHAR* argv[])
{
	int mode = 0;
	_stscanf(argv[1], _T("%d"), &mode);
	if (mode == 0)
	{
#ifdef _DEBUG
		printf("DEBUG mode: results will be written to the file (Task1_output.txt)\n\n");
		DeleteFile(L"Task1_output.txt");
#else
		printf("RELEASE mode: results will not be written to the file\n\n");
#endif // _DEBUG
		for (int i = 0; i < 10; i++)
		{
#ifdef _DEBUG
			HANDLE h = CreateFileForWrite(L"Task1_output.txt");
			DWORD written;
			TCHAR t1[20];
			_stprintf_s(t1, L"Start process %d\r\n", i);
			DWORD size = _tcslen(t1)*sizeof(TCHAR);
			SetFilePointer(h, 0, 0, FILE_END);
			WriteFile(h, &t1, size, &written, 0);
			if (written != size) {
				_tprintf(_T("Start-write-error\n"));
				return -1;
			}		
			CloseHandle(h);
#endif // _DEBUG
			printf("Start process %d\n", i);
			for (int i = 0; i < 1000000; i++);
#ifdef _DEBUG
			h = CreateFileForWrite(L"Task1_output.txt");
			TCHAR t2[20];
			_stprintf_s(t2, L"End process %d\r\n", i);
			size = _tcslen(t2)*sizeof(TCHAR);
			SetFilePointer(h, 0, 0, FILE_END);
			WriteFile(h, &t2, size, &written, 0);
			if (written != size) {
				_tprintf(_T("End-write-error\n"));
				return -1;
			}			
			CloseHandle(h);
#endif
			printf("End process %d\n", i);
		}
		return 0;
	}
	else
	{
#ifdef _DEBUG
		printf("DEBUG mode: results will be written to the file (Task1_output.txt)\n\n");
		DeleteFile(L"Task1_output.txt");
#else
		printf("RELEASE mode: results will not be written to the file\n\n");
#endif // _DEBUG
		int numbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		HANDLE hs[10];
		for (int i = 0; i < 10; i++)
		{
			hs[i] = MyCreateThread(threadFuncTask1, &numbers[i], 0);
		}
		DWORD prN = WaitForMultipleObjects(10, hs, FALSE, INFINITE);
		printf("\nFirst thread is finished. Thread #%d\nCalls count: %d\n\n", prN, getCount());
		WaitForMultipleObjects(10, hs, TRUE, INFINITE);
		printf("\nAll threads were finished.\nCalls count: %d\n\n", getCount());
		return 0;
	}

}

