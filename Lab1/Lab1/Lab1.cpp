// Lab1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <locale.h>
#include <windows.h>
#include <stdio.h>
#include <io.h>

#define DEFAULT_FILE "words.txt"
#define _WDEFAULT_FILE L"words.txt"

#pragma warning(disable : 4996 )

int checkEncoding()
{
	char* strEncoding;
	if ((int)sizeof(TCHAR) == 1)
		strEncoding = "ASCII";
	else
		strEncoding = "UNICODE";
	printf("Current encoding: %s || Size of TCHAR = %d\n\n", strEncoding, (int)sizeof(TCHAR));
	return (int)sizeof(TCHAR);
}

void printFamily()
{
	char father[] = "Иорданов Алексей Александрович";
	char mother[] = "Иорданова Наталия Юриевна";

	printf("My family with ASCII encoding: \n\t%s\n\t%s\n", father, mother);

	WCHAR Wfather[sizeof(father)];
	WCHAR Wmother[sizeof(mother)];

	MultiByteToWideChar(CP_ACP, 0, father, strlen(father) + 1, Wfather, sizeof(Wfather));
	MultiByteToWideChar(CP_ACP, 0, mother, strlen(mother) + 1, Wmother, sizeof(Wmother));

	wprintf(L"My family with UNICODE encoding: \n\t%s\n\t%s\n", Wfather, Wmother);
	_tprintf(_T("My family with local encoding: \n\t%s\n\t%s\n\n"), Wfather, Wmother);

	printf("Press any key to show ASCII message box..");
	getchar();
	MessageBoxA(0, father, "Using ASCII encoding", MB_OK);
	printf("Shown\n");

	printf("Press any key to show UNICODE message box..");
	getchar();
	MessageBoxW(0, Wfather, L"Using UNICODE encoding", MB_OK);
	printf("Shown\n\n");
}

static int compare(const void *a, const void *b)
{
	//char *l = (char*)a;
	//char *r = (char*)b;
	//return strncmp(l, r, 4);
	WCHAR *l = *(WCHAR **)a;
	WCHAR *r = *(WCHAR **)b;
	return wcsncmp(l, r, 4);
}

void sortStringArray()
{
	WCHAR* arr[] = { L"bca", L"addb", L"mxnzacb" };
	printf("Unsorted array (UNICODE): ");
	for (int i = 0; i < sizeof(arr) / sizeof(WCHAR*); i++)
	{
		wprintf(L"%s ", arr[i]);
	}
	qsort(arr, sizeof(arr) / sizeof(WCHAR*), 4, compare);
	printf("\n\nSorted array (UNICODE): ");
	for (int i = 0; i < sizeof(arr) / sizeof(WCHAR*); i++)
	{
		wprintf(L"%s ", arr[i]);
	}
	printf("\nSorted array (ASCII): ");
	for (int i = 0; i < sizeof(arr) / sizeof(WCHAR*); i++)
	{
		char* res = (char*) malloc(wcslen(arr[i]) + 1);
		WideCharToMultiByte(CP_ACP, 0, arr[i], wcslen(arr[i]) + 1, res, strlen(res) + 1, 0, 0);
		printf("%s ", res);
	}
	printf("\n\n");
}

void reverseFile()
{
	FILE* file = fopen(DEFAULT_FILE, "rt");
	if (file != NULL) 
	{
		int i = 0;
		BYTE buf[1000];
		int num = fread(buf, 1, 1000, file);
		fclose(file);

		file = fopen(DEFAULT_FILE, "rt");
		int lSize = ftell(file);
		WCHAR buf2[1000];
		int num2 = fread(buf2, 2, 1000, file);
		fclose(file);

		if (IsTextUnicode(buf, num, 0))
		{
			printf("File encoding: UNICODE");
			WCHAR arr[sizeof(buf2) / sizeof(WCHAR)] = {};
			int n = 0;
			int last = 0;
			while (n < num2)
			{
				if (buf2[n] == '\r' || buf2[n] == '\n' || buf2[n] == 65279)
	
				{
					int j = last;
					for (int i = n - 1; i >= last; i--)
					{
						arr[j] = (WCHAR)buf2[i];
						j++;
					}
					if (buf2[n] == '\r')
						arr[j] = '\r';
					else if (buf2[n] == '\n')
						arr[j] = '\n';
					else if (buf2[n] == 65279)
						arr[j] = 65279;
					last = n + 1;
				}
				else if (n == num2 - 1)
				{
					int j = last;
					for (int i = n; i >= last; i--)
					{
						arr[j] = (WCHAR)buf2[i];
						j++;
					}
				}
				n++;
			}
			WCHAR res[sizeof(buf2) / sizeof(WCHAR)] = {};
			int j = 0;
			for (int i = 0; i < sizeof(arr) / sizeof(WCHAR); i++)
			{
				if ((arr[i] == 65279 && arr[i + 1] == 65279) || (arr[i] == '\r' && arr[i + 1] == '\r'))
					num2 -= 1;
				else
					res[j++] = arr[i];
			}
			file = _wfopen(_WDEFAULT_FILE, L"w, ccs=UTF-16LE");
			fwrite(res, 2, num2, file);
			wprintf(L"\nReversed text from file:\n%s", res);
			fclose(file);
		}
		else
		{
			printf("File encoding: ASCII");
			char arr[sizeof(buf) / sizeof(byte)] = {};
			int n = 0;
			int last = 0;
			while (n < num)
			{
				if (buf[n] == '\r' || buf[n] == '\n')
				{
					int j = last;
					for (int i = n-1; i >= last; i--)
					{
						arr[j] = (char)buf[i];
						j++;
					}
					if (buf[n] == '\r')
						arr[j] = '\r';
					else if(buf[n] == '\n')
						arr[j] = '\n';
					last = n + 1;
				}
				else if (n == num - 1)
				{
					int j = last;
					for (int i = n; i >= last; i--)
					{
						arr[j] = (char)buf[i];
						j++;
					}
				}
				n++;
			}
			file = fopen(DEFAULT_FILE, "w");
			fwrite(arr, 1, num, file);
			printf("\nReversed text from file:\n%s", arr);
			fclose(file);
		}
		
	}
	else
	{
		printf("File cannot be opened or created!");
	}
}

int main()
{
	printf("Step 1. Checking locale encoding (press any key)\n");
	getchar();
	checkEncoding();

	printf("-----------------------------------------------------------------\n");
	printf("Step 2. Changing charset for ASCII (press any key)\n");
	getchar();
	if (_tsetlocale(LC_ALL, _T("RUSSIAN")) == 0)
		printf("Cannot change language\n\n\n");
	else
		printf("Current language for local encoding: Russian\n\n");

	printf("-----------------------------------------------------------------\n");
	printf("Step 3. Printing family with different encoding (press any key)\n");
	getchar();
	printFamily();
	
	printf("-----------------------------------------------------------------\n");
	printf("Step 4. Sorting the array of strings (press any key)\n");
	getchar();
	sortStringArray();

	printf("-----------------------------------------------------------------\n");
	printf("Step 5. Reversing strings in file (press any key)\n");
	getchar();
	reverseFile();
	printf("\n\nProgram was successfuly complited\nPress any key to exit..");
	getchar();
    return 0;
}

