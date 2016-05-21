// NotepadRunner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\OS_lab_5\Macroses.h"


int _tmain()
{	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR proc[] = _T("Notepad.exe");
	BOOL b = CreateUnsuspendedProcess(proc, 0, &si, &pi);

	if (b) {
		cout << "Notepad process is executing.." << endl;
		WaitForSingleObject(pi.hProcess, INFINITE);

		DWORD code;
		GetExitCodeProcess(pi.hProcess, &code);
		if(code == 0)
			cout << "Process successfuly finished" << endl;

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return 0;
}

