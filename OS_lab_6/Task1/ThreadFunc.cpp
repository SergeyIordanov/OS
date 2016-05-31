#include "stdafx.h"
#include <Windows.h>
#include "ThreadFunc.h"
#include "../OS_lab_6/myHeader.h"

volatile unsigned countCalls = 0;
TCHAR* fileName = L"Task1_output.txt";

#ifdef _DEBUG
DWORD WINAPI threadFuncTask1(PVOID par) {
	HANDLE hMutex = CreateMutex(0, FALSE, _T("mutex!"));

	countCalls++;
	int temp = *(int*)par;

	WaitForSingleObject(hMutex, INFINITE);
	HANDLE h = CreateFileForWrite(fileName);	
	DWORD written;
	TCHAR t1[20];
	_stprintf_s(t1, L"Start process %d\r\n", temp);
	DWORD size = _tcslen(t1)*sizeof(TCHAR);
	SetFilePointer(h, 0, 0, FILE_END);
	WriteFile(h, &t1, size, &written, 0);
	if (written != size) {
		_tprintf(_T("Start-write-error\n"));
		return -1;
	}
	printf("Start process %d\n", temp);
	CloseHandle(h);
	ReleaseMutex(hMutex);
	

	for (int i = 0; i < 10000000; i++);

	WaitForSingleObject(hMutex, INFINITE);
	h = CreateFileForWrite(fileName);
	TCHAR t2[20];
	_stprintf_s(t2, L"End process %d\r\n", temp);
	size = _tcslen(t2)*sizeof(TCHAR);
	SetFilePointer(h, 0, 0, FILE_END);
	WriteFile(h, &t2, size, &written, 0);
	if (written != size) {
		_tprintf(_T("End-write-error\n"));
		return -1;
	}
	printf("End process %d\n", temp);
	CloseHandle(h);
	ReleaseMutex(hMutex);
	

	CloseHandle(hMutex);
	return 0;
}
#else
DWORD WINAPI threadFuncTask1(PVOID par) {
	++countCalls;
	int temp = *(int*)par;
	printf("Start process %d\n", temp);
	for (int i = 0; i < 1000000; i++);
	printf("End process %d\n", temp);
	return 0;
}
#endif

unsigned getCount()
{
	return countCalls;
}