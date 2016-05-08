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
	DWORD dwCount;

public:
	MailBox(TCHAR* fname, DWORD maxSize = 0);
	~MailBox() {}

	BOOL AddLetter(BYTE* message, DWORD size);
	BOOL ReadLetter(DWORD idLetter, BYTE* &message, DWORD &size);
	DWORD DeleteLetter(DWORD idLetter);
	void ReadUsingFileMapping();
	DWORD getMessageCount() { return lettersCount; }
	TCHAR* getFileName() { return fileName; }
	DWORD getMaxSize() { return maxSize; }
	DWORD getMailboxesCount() { return mailBoxesCount; }
	DWORD getDwCount() { return dwCount; }
private:
	BOOL setCRC16(TCHAR* fname);
	BOOL checkCRC16(TCHAR* fname);
	UINT16 calcCRC16(UINT8 *buf, DWORD sizeOfData);
};