// RSA_LIB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RSA_LIB.h"
#include <stdlib.h>
#include <time.h>

unsigned getNearestSimple(unsigned x)
{
	unsigned size = x;
	unsigned i, j;
	BOOL* mas = (BOOL*)calloc(size + 1, sizeof(BOOL));

	for (i = 0; i <= size; i++)
		mas[i] = 1;

	for (i = 2; ((i*i) <= size); i++)
		if (mas[i])
			for (j = (i*i); j <= size; j += i)
				if (mas[j])
					mas[j] = 0;

	for (i = size - 1; i > 0; i--)
		if (mas[i] == 1)
			return i;
	return 0;
}

unsigned simple() {
	unsigned res;
	srand((unsigned) time(0));
	res = 100 + rand() % 150;
	res = getNearestSimple(res);
	return res;
}

unsigned gcd(unsigned a, unsigned b)
{
	if(a < b)
		return (a == 0) ? b : gcd(b % a, a);
	else
		return (b == 0) ? a : gcd(a % b, b);
}

unsigned fastPowerByModule(unsigned a, unsigned e, unsigned n)
{
	unsigned b = 1;

	while (e) {
		if (e % 2 == 0) {
			e /= 2;
			a = (a*a)%n;
		}
		else {
			e--;
			b = (b*a)%n;
		}
	}
	return b;
}

unsigned keys(unsigned &d, unsigned &e) {

	unsigned p = simple();
	unsigned q;
	do {
		q = simple();
	} while (p == q);
	unsigned n = p*q;
	unsigned fi = (p - 1) * (q - 1);
	for (e = 3; gcd(e, fi) != 1; e += 2);
	for (d = 2; ((d * e) % fi) != 1; d++);
	return n;
}

unsigned ed(unsigned a, unsigned key, unsigned n) {
	unsigned res = 1;
	res = fastPowerByModule(a, key, n);
	return res;
}

