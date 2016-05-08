#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include "Methods.h"
#include <locale>
#include <list>

using namespace std;


struct memPage {
	LPVOID address;
	SIZE_T size;
	DWORD state;
};

vector<memPage> freeMem;
vector<memPage> buzyMem;

MEMORY_BASIC_INFORMATION mb;
SIZE_T dwLength = sizeof(mb);
HANDLE h = GetCurrentProcess();

SIZE_T bestSuitedSize, bestSuitedIndex;
SYSTEM_INFO si;

void getSystemInfo() {
	GetSystemInfo(&si);	
	VirtualQueryEx(h, si.lpMinimumApplicationAddress, &mb, dwLength);
}

//------First task------
void printVirtualMemoryInfo() {
	MEMORYSTATUSEX ms;
	ms.dwLength = sizeof(ms);
	GlobalMemoryStatusEx(&ms);

	printf("Total virtual memory = %d\n", (int)ms.ullTotalVirtual);
	printf("Available virtual memory = %d\n", (int)ms.ullAvailVirtual);
}

void getMemoryInfo() {
	getSystemInfo();
	printf("\n");
	printf("----System & virtual memory info----\n");
	printf("Minimum application address: = %08x\n", (unsigned)si.lpMinimumApplicationAddress);
	printf("Maximum application address: = %08x\n", (unsigned)si.lpMaximumApplicationAddress);
	printf("Allocation granularity: = %d\n", (int)si.dwAllocationGranularity);
	printf("Page size: = %d\n", si.dwPageSize);
	printf("\n");
	printVirtualMemoryInfo();
	printf("------------------------------------\n");
	printf("\n");
}

//------Second task------
void printMemoryBlocks(vector<memPage> &mas)
{
	for (unsigned i = 0; i < mas.size(); i++) {
		printf("BaseAddress: %x (%d)\n", (unsigned)mas[i].address, (unsigned)mas[i].address);
		printf("RegionSize: %d\n", (unsigned)mas[i].size);
		printf("State: ");
		switch (mas[i].state) {
		case MEM_COMMIT:
			printf("MEM_COMMIT");
			break;
		case MEM_FREE:
			printf("MEM_FREE");
			break;
		case MEM_RESERVE:
			printf("MEM_RESERVE");
			break;
		}
		printf("\n\n");
	}
}

void printFreeMemoryBlocks() {

	LPVOID start = si.lpMinimumApplicationAddress;
	LPVOID stop = si.lpMaximumApplicationAddress;

	printf("\n----Free memory blocks----\n");

	printMemoryBlocks(freeMem);
	printf("--------------------------\n\n");
}

void printBuzyMemoryBlocks()
{
	LPVOID start = si.lpMinimumApplicationAddress;
	LPVOID stop = si.lpMaximumApplicationAddress;

	printf("\n----Buzy memory blocks----\n");
	printMemoryBlocks(buzyMem);
	printf("--------------------------\n\n");
}

void fillVectors()
{
	freeMem.clear();
	buzyMem.clear();

	LPVOID start = si.lpMinimumApplicationAddress;
	LPVOID stop = si.lpMaximumApplicationAddress;
	while ((unsigned)start < (unsigned)stop)
	{
		VirtualQueryEx(h, start, &mb, dwLength);

		if (mb.State != MEM_FREE)
		{
			memPage buzyPage;
			buzyPage.address = start;
			buzyPage.size = mb.RegionSize;
			buzyPage.state = mb.State;
			buzyMem.push_back(buzyPage);
		}
		else
		{
			memPage freePage;
			freePage.address = start;
			freePage.size = mb.RegionSize;
			freePage.state = mb.State;
			freeMem.push_back(freePage);
		}
		start = (BYTE*)start + mb.RegionSize;
	}
}

//Выделение памяти
void theLeastSufficientAdd(SIZE_T size) {
	bestSuitedSize = -1;
	bestSuitedIndex = -1;

	// Ищем куда добавить
	for (unsigned i = 0; i < freeMem.size(); i++)
	{
		if ((freeMem[i].size >= size) && (bestSuitedIndex == -1 || freeMem[i].size < bestSuitedSize))
		{
			bestSuitedSize = freeMem[i].size;
			bestSuitedIndex = i;
		}
	}

	if (bestSuitedIndex == -1)
	{
		printf("\nNo suitable region.\n\n");
		return;
	}

	printf("\nMemory found:\nSize: %d\nAddress: %x\n\n", (unsigned)bestSuitedSize, (unsigned)freeMem[bestSuitedIndex].address);

	VirtualAllocEx(h, freeMem[bestSuitedIndex].address, size, MEM_COMMIT, 0);

	memPage temp;
	temp.address = freeMem[bestSuitedIndex].address;
	temp.size = size;
	temp.state = MEM_COMMIT;
	buzyMem.push_back(temp);
	freeMem.erase(freeMem.begin() + bestSuitedIndex);
}

//Освобождение памяти
void theLeastSufficientDelete(LPVOID del)
{
	BOOL ok = false;
	for (unsigned i = 0; i < buzyMem.size(); i++)
	{
		if (buzyMem[i].address == del)
		{
			VirtualFreeEx(h, buzyMem[i].address, 0, MEM_DECOMMIT);
			memPage freePage;
			freePage.address = buzyMem[i].address;
			freePage.size = buzyMem[i].size;
			freePage.state = MEM_FREE;
			freeMem.push_back(freePage);
			buzyMem.erase(buzyMem.begin() + i);
			printf("\nMemory by address %x is free now\n\n", (unsigned)buzyMem[i].address);
			ok = true;
			break;
		}
	}
	if (!ok)
	{
		printf("\nCannot make free memory by address %x\n\n", (unsigned)del);
	}
}
list<PAGE> pageList;

void addPage(PVOID *pA, SIZE_T count, pPage p) {
	BOOL found = false;
	PAGE temp;
	list<PAGE>::iterator i;
	for (i = pageList.begin(); i != pageList.end(); i++) {
		if (i->virt == p->virt) {
			found = true;
			p->phys = i->phys;
			temp.virt = p->virt;
			temp.phys = p->phys;
			pageList.erase(i);
			break;
		}
	}
	if (!found) {
		if (pageList.size() < count) {
			temp.virt = p->virt;
			temp.phys = pA[pageList.size()];
			p->phys = pA[pageList.size()];
		}
		else {
			temp = *pageList.begin();
			p->phys = temp.phys;
			pageList.erase(pageList.begin());
		}
	}
	pageList.push_back(temp);
}

void printPageList() {
	printf("\nCurrent page list:\n");
	list<PAGE>::iterator i;
	for (i = pageList.begin(); i != pageList.end(); i++) {
		printf("Virt = %x Phys = %x\n", (unsigned)i->virt, (unsigned)i->phys);
	}
	printf("\n");
}

//------Third task------
vector<SIZE_T> limitedVector;
void printLimitedVector()
{
	for (unsigned i = 0; i < limitedVector.size(); i++) {
		_tprintf(_T("%d. %d\n"), i, limitedVector[i]);
	}
	_tprintf(_T("\n"));
}

void addToPages(SIZE_T elem)
{
	_tprintf(_T("Elem: %d\n"), elem);
	SIZE_T maxLen = 5;

	for (unsigned i = 0; i < limitedVector.size(); i++)
	{
		if (limitedVector[i] == elem)
		{
			limitedVector.push_back(limitedVector[i]);
			limitedVector.erase(limitedVector.begin() + i);
			printLimitedVector();
			return;
		}
	}

	if (limitedVector.size() == maxLen)
	{
		limitedVector.erase(limitedVector.begin());
		limitedVector.push_back(elem);
	}
	else
	{
		limitedVector.push_back(elem);
	}
	printLimitedVector();
}
