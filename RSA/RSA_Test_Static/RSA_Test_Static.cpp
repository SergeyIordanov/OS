// RSA_Test_Static.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../RSA_LIB/RSA_LIB.h"


int main()
{
	unsigned e, d, n;
	printf("Using static way of connecting library\n---------------------------------------\n\n");
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

