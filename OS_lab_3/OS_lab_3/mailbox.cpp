#include "stdafx.h"
#include "mailbox.h"

#pragma warning(disable : 4996 )

MailBox::MailBox(TCHAR * fname, DWORD maxSize)
{
	BOOL b = false;
	DWORD dwCount = 0; // <---- WHAT A FUCK IS IT??
	if (maxSize == 0) {
		HANDLE h = CreateFileForRead(fname);
		b = h != IHV;
		if (!b) throw Errors(_T("File doesn't exist"));		
		b = ReadFile(h, &this->lettersCount, sizeof(this->lettersCount), &dwCount, 0);
		if (!b) throw Errors(_T("Cannot read file"));
		b = ReadFile(h, &maxSize, sizeof(maxSize), &dwCount, 0);
		_tcscpy(this->fileName, fname);
		CloseHandle(h);
		//mailBoxesCount++;
	}
	else {
		HANDLE h = CreateFileForWrite(fname);
		if (h == IHV) throw Errors(_T("File already exists"));
		this->lettersCount = 0;
		this->maxSize = maxSize;
		b = WriteFile(h, &this->lettersCount, sizeof(this->lettersCount), &dwCount, 0);
		if(!b) throw Errors(_T("Cannot write file"));
		b = WriteFile(h, &maxSize, sizeof(this->maxSize), &dwCount, 0);
		//MailBox::mailBoxesCount = 1;
	}
}

BOOL MailBox::AddLetter(BYTE * message, DWORD size)
{
	return 0;
}

BOOL MailBox::ReadLetter(DWORD idLetter, BYTE * message, DWORD * size)
{
	return 0;
}

DWORD MailBox::DeleteLetter(DWORD idLetter)
{
	return 0;
}
