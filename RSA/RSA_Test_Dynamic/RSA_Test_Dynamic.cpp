// RSA_Test_Dynamic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

typedef unsigned(*KEYS) (unsigned &d, unsigned &e);
typedef unsigned(*ED) (unsigned a, unsigned key, unsigned n);

int main()
{
	HMODULE h = LoadLibrary(_T("RSA_LIB.dll"));
	if (h != 0) {
		KEYS keys = (KEYS)GetProcAddress(h, "keys");
		ED ed = (ED)GetProcAddress(h, "ed");
		if (keys && ed) {
			unsigned e, d, n;
			printf("Using dynamic way of connecting library\n---------------------------------------\n\n");
			printf("Generating keys...\n\n");
			n = keys(d, e);
			printf("Open key: e = %d, n = %d\n", e, n);
			printf("Secret key: d = %d, n = %d\n\n", d, n);
			for (unsigned a = 5; a <= 10; a++) {
				printf("Original: %d\n", a);
				unsigned ae = ed(a, e, n);
				printf("Encrypted: %d\n", ae);
				unsigned ad = ed(ae, d, n);
				printf("Decrypted: %d\nPress for the next..\n", ad);
				getchar();
			}

			_wsetlocale(LC_ALL, L"Russian");
			wchar_t* string = L"Hello World! Привет мир!";
			printf("String\tNumEquivalent\tEncryptedNum\tDecryptedNum\tDecryptedString\n");
			for (unsigned i = 0; i < wcslen(string) + 1; i++) {
				unsigned ae = ed((unsigned)string[i], e, n);
				unsigned ad = ed(ae, d, n);
				wchar_t temp = (wchar_t)ad;
				wprintf(L"%6c\t%13d\t%12d\t%12d\t%15c\n", string[i], (unsigned)string[i], ae, ad, temp);
			}
			getchar();
			return 0;
		}
		else {
			printf("Cannot import functions.");
		}
	}
	else {
		printf("Cannot load library.");
	}
    return 0;
}

