#include "stdafx.h"
#include <Windows.h>
#include <vector>

typedef struct {
	PVOID virt;
	PVOID phys;
} PAGE, *pPage;

void getSystemInfo();

void getMemoryInfo();

void fillVectors();

void theLeastSufficientAdd(SIZE_T added);

void theLeastSufficientDelete(LPVOID del);

void printLimitedVector();

void addToPages(SIZE_T elem);

void printVirtualMemoryInfo();

void printFreeMemoryBlocks();

void printBuzyMemoryBlocks();

void addPage(PVOID *pA, SIZE_T count, pPage p);

void printPageList();