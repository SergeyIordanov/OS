// ProcessesInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../OS_lab_5/Macroses.h"
#include <tlhelp32.h>
#include <psapi.h>

int main()
{
	HANDLE hModSnap;
	HANDLE hProcSnap;

	PROCESSENTRY32 procEntry; // proccess info
	MODULEENTRY32 modEntry; // module info
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	modEntry.dwSize = sizeof(MODULEENTRY32);

	DWORD procCount = 1, modCount = 1;

	// Creating descriptor to access process' information
	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == IHV)
	{
		_tprintf(_T("Cant get access to process information!"));
		return 0;
	}

	// Get the first process' info
	if (!Process32First(hProcSnap, &procEntry))
	{
		_tprintf(_T("Cant get process information!"));
		return 0;
	}

	while (Process32Next(hProcSnap, &procEntry))
	{
		_tprintf(_T("%d. PROCESS %s WITH PID %d, HAS %d THREAD(-S)\n"), procCount++, procEntry.szExeFile, procEntry.th32ProcessID, procEntry.cntThreads);

		// Creating descriptor to access module's information of current proccess
		hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procEntry.th32ProcessID);
		if (hModSnap == IHV)
		{
			printf("\n-------------------------------------------\n");
			continue;
		}
		modEntry.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hModSnap, &modEntry))
		{
			_tprintf(_T("Cant get process information!"));
			return 0;
		}
		while (Module32Next(hModSnap, &modEntry))
		{
			_tprintf(_T("\t%d  %s\n"), modCount++, modEntry.szModule);
		}
		modCount = 1;
	}
	CloseHandle(hProcSnap);
	return 0;
}

