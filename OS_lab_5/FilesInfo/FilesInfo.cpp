// FilesInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\OS_lab_5\Macroses.h"


BOOL IsUnicode(PBYTE file)
{
	return(file[0] == 0xfe && file[1] == 0xff) || (file[0] == 0xff && file[1] == 0xfe);
}

template<typename T>
void countStrings(T* str, int size)
{
	int index = 0;
	int count = 0;
	int length = 0;

	while (index < size)
	{
		if (str[index] == '\r' && str[index + 1] == '\n' && index + 1 != size)
		{
			_tprintf(_T("\tString #%d has %d characters.\n"), ++count, length);
			length = 0;
			index += 2;
			continue;
		}
		length++;
		index++;
	}

	if (length != 0) {
		_tprintf(_T("\tString #%d has %d characters.\n"), ++count, length);
	}

}



void fileInfo(TCHAR* fileName) {

	HANDLE h = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (h != IHV)
	{
		DWORD dwSize = GetFileSize(h, 0);
		PBYTE buf = new BYTE[dwSize];
		DWORD dwCount;
		ReadFile(h, buf, dwSize, &dwCount, 0);
		CloseHandle(h);

		BOOL isUnicode = IsUnicode(buf);
		_tprintf(_T("FileName: %s\nFileSize: %d\nFileEncoding %s\n"), fileName, dwSize, (isUnicode ? _T("UNICODE") : _T("ANSI")));
		if (isUnicode)
		{
			wchar_t* buf_wc = (wchar_t*)buf;
			buf_wc++;
			countStrings(buf_wc, dwSize / 2 - 1);
			delete[] --buf_wc;
		}
		else
		{
			char* buf_c = (char *)buf;
			countStrings(buf_c, dwSize);
			delete[] buf_c;
		}
		printf("\n");
	}
}

#pragma warning( disable : 4996 )
int _tmain(int argc, TCHAR **argv)
{
	WIN32_FIND_DATA fd;
	if (argc == 3)
	{
		FILETIME t1;
		_stscanf(argv[1], _T("%d"), &t1.dwLowDateTime);
		_stscanf(argv[2], _T("%d"), &t1.dwHighDateTime);

		TCHAR *templ = _T("*.txt");
		HANDLE h = FindFirstFile(templ, &fd);

		if (h != IHV) {
			do {
				if (CompareFileTime(&t1, &fd.ftLastWriteTime) < 0) {
					fileInfo(fd.cFileName);
				}
			} while (FindNextFile(h, &fd));
			FindClose(h);
		}
	}
	return 0;
}

