#pragma once

#include "stdafx.h"
#include <Windows.h>
#include "file.h"

class MailBox
{
	TCHAR* fileName;
	DWORD lettersCount;
	DWORD maxSize;
	static DWORD mailBoxesCount;
public:
	MailBox(TCHAR* fname, DWORD maxSize = 0);
	~MailBox() {}

	BOOL AddLetter(BYTE* message, DWORD size);
	BOOL ReadLetter(DWORD idLetter, BYTE* message, DWORD* size);
	DWORD DeleteLetter(DWORD idLetter);
	DWORD getMessageCount() { return lettersCount; }
	TCHAR* getFileName() { return fileName; }
	DWORD getMaxSize() { return maxSize; }
	DWORD getMailboxesCount() { return mailBoxesCount; }

	class Errors
	{
	public:
		Errors(TCHAR* msg) { _tprintf(msg); }
		~Errors() {}
	};
};