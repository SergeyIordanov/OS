// OS_lab_4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Methods.h"
#include <Windows.h>
#include "LRU.h"

PVOID pA[] = {
	(PVOID)0x40000,
	(PVOID)0x40001,
	(PVOID)0x40002,
	(PVOID)0x40003,
	(PVOID)0x40004
};
SIZE_T countMem = sizeof(pA) / sizeof(PVOID);
PVOID seq[]{ (PVOID)1, (PVOID)3, (PVOID)7, (PVOID)5, (PVOID)4, (PVOID)9, (PVOID)3,
(PVOID)5, (PVOID)4, (PVOID)3, (PVOID)1, (PVOID)5, (PVOID)5, (PVOID)2 };

void genLRU() {
	startLRU();
	printf("Array: 4, 1, 0, 2, 3, 0, 3, 1\n");
	int arr[] = { 4, 1, 0, 2, 3, 0, 3, 1 };
	int size = sizeof(arr) / sizeof(int);
	for (int i = 0; i < size; i++)
	{
		put(arr[i]);
		printCache();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Choose operation:\n");
	size_t c = -1;
	int count = 0;
	int n = 0;

	getSystemInfo();
	fillVectors();
	while (c != 0) {
		_tprintf(_T("0. Close the program.\n"));
		_tprintf(_T("1. Print system & virtual memory information\n"));
		_tprintf(_T("2. Print the list of free memory blocks\n"));
		_tprintf(_T("3. Print the list of buzy memory blocks\n"));
		_tprintf(_T("4. Allocate memory\n"));
		_tprintf(_T("5. Free memory\n"));
		_tprintf(_T("6. Page rebase algorithm\n"));
		_tprintf(_T("7. LRU\n"));
		_tscanf_s(_T("%d"), &c);
		size_t j = -1;
		switch (c)
		{
		case 0:
			_tprintf(_T("Exiting..\n"));
			break;
		case 1:
			getMemoryInfo();
			break;
		case 2:
			printFreeMemoryBlocks();
			break;
		case 3:
			printBuzyMemoryBlocks();
			break;
		case 4:
			_tprintf(_T("Enter memory size:\n"));
			_tscanf_s(_T("%d"), &j);
			theLeastSufficientAdd((SIZE_T)j);
			break;
		case 5:
			_tprintf(_T("Enter region address for deleting:\n"));
			_tscanf_s(_T("%d"), &j);
			theLeastSufficientDelete((LPVOID)j);
			break;
		case 6:
			n = sizeof(seq) / sizeof(seq[0]);
			printf("Stack:\n");
			for (int i = 0; i < n; i++) {
				PAGE p;
				p.virt = seq[i];
				addPage(pA, countMem, &p);
				printf("\nVirt = %x Phys = %x", (unsigned)p.virt, (unsigned)p.phys);
			}
			printf("\n");
			printPageList();
			break;
		case 7:
			genLRU();
			break;
		}
	}
	return 0;
}

