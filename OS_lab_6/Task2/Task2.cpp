//// Task2.cpp : Defines the entry point for the console application.
////
//
//#include "stdafx.h"
//#include <Windows.h>
//#include "../OS_lab_6/myHeader.h"
//#include "Print.h"
//
//volatile unsigned countCalls = 0;
//
//DWORD WINAPI threadFuncTask2(PVOID par)
//{
//	HANDLE hMutex = CreateMutex(0, FALSE, _T("mutex"));
//	InterlockedIncrement(&countCalls);
//	ReleaseMutex(hMutex);
//	CloseHandle(hMutex);
//	return 0;
//}
//
//int main()
//{
//	//if(!PrintHeader())return 0;
//	HANDLE* h;
//	for (unsigned i = 2;; i *= 2) {
//		countCalls = 0;
//		h = new HANDLE[i];
//		for (unsigned j = 0; j < i; j++)
//		{
//			h[j] = MyCreateThread(threadFuncTask2, 0, 0);
//		}
//
//		WaitForMultipleObjects(i, h, TRUE, INFINITE);
//
//		for (unsigned j = 0; j < i; j++)
//		{
//			CloseHandle(h[j]);
//		}
//		delete[] h;
//		printf("Count(i): %d Threads: %d\n", i, countCalls);
//		if (countCalls < i)
//		{
//			printf("Max number of threads: %d\n\n", countCalls);
//			break;
//		}
//	}
//    return 0;
//}

#include "stdafx.h"
#include <windows.h>

volatile unsigned countTask2 = 0;

DWORD WINAPI threadCallTask2(LPVOID par) {
	//HANDLE hMutex = CreateMutex(0, FALSE, _T("MyMutex!"));
	InterlockedIncrement(&countTask2);
	//ReleaseMutex(hMutex);
	return 0;
}

void countMaxThreads() {

	for (size_t i = 2;; i++)
	{
		HANDLE* h = new HANDLE[i];
		InterlockedExchange(&countTask2, 0);

		for (size_t j = 0; j < i; j++)
		{
			_tprintf(L"\tCount: %d i:%d j: %d\n", countTask2, i, j);
			h[j] = CreateThread(0, 0, threadCallTask2, 0, 0, 0);

		}		
		WaitForMultipleObjects(i, h, TRUE, INFINITE);
		for (int j = 0; j < i; j++) {
			CloseHandle(h[j]);
		}
		delete[]h;

		if (countTask2 != i) {
			_tprintf(L"Max threads number: %d\n", countTask2);
			break;
		}
	}
}

int main(){
	countMaxThreads();
	return 0;
}