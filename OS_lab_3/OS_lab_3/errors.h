#pragma once

#include "stdafx.h"

class Errors
{
	TCHAR msg;
public:
	Errors(TCHAR* msg) { _tprintf(msg); }
	~Errors() {}
};