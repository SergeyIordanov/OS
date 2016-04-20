#include "stdafx.h"
#include "mailbox.h"
#include "errors.h"
#include <malloc.h>

#pragma warning(disable : 4996 )

DWORD MailBox::mailBoxesCount = 0;

MailBox::MailBox(TCHAR * fname, DWORD maxSize)
{
	BOOL b = false;
	if (maxSize == 0) {
		HANDLE h = CreateFileForRead(fname);
		b = h != IHV;
		if (!b) throw Errors(_T("File doesn't exist"));		
		b = ReadFile(h, &this->lettersCount, sizeof(this->lettersCount), &this->dwCount, 0);
		if (!b) throw Errors(_T("Cannot read file"));
		b = ReadFile(h, &this->maxSize, sizeof(this->maxSize), &this->dwCount, 0);
		if (!b) throw Errors(_T("Cannot read file"));
		//_tcscpy(this->fileName, fname);
		this->fileName = fname; //WARNING!!!
		CloseHandle(h);
		MailBox::mailBoxesCount ++; // количество открытых ящиков в процессе работы программы
	}
	else {
		HANDLE h = CreateFileForWrite(fname);
		if (h == IHV) throw Errors(_T("File already exists"));
		this->lettersCount = 0;
		this->maxSize = maxSize;
		this->fileName = fname; //WARNING!!!
		b = WriteFile(h, &this->lettersCount, sizeof(this->lettersCount), &this->dwCount, 0);
		if(!b) throw Errors(_T("Cannot write file"));
		b = WriteFile(h, &maxSize, sizeof(this->maxSize), &this->dwCount, 0);
		if (!b) throw Errors(_T("Cannot write file"));
		CloseHandle(h);
		MailBox::mailBoxesCount++;
	}
}

// структура файла: [lettersCount][maxSize][letter1][letter2]...

BOOL MailBox::AddLetter(BYTE * message, DWORD size)
{
	HANDLE h = OpenFileForWrite(this->fileName);
	if(h == IHV) throw Errors(_T("File doesn't exist"));
	DWORD fileSize = GetFileSize(h, 0);
	if(fileSize + size + sizeof(DWORD) > maxSize) throw Errors(_T("No more free space in current mailbox"));
	this->lettersCount++;
	BOOL b = WriteFile(h, &lettersCount, sizeof(lettersCount), &this->dwCount, 0);
	if(!b) throw Errors(_T("Cannot write message"));
	SetFilePointer(h, 0, 0, FILE_END);
	b = WriteFile(h, &size, sizeof(size), &this->dwCount, 0);
	if(!b) throw Errors(_T("Cannot write message"));
	b = WriteFile(h, message, size, &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot write message"));
	CloseHandle(h);
	return true;
}

BOOL MailBox::ReadLetter(DWORD idLetter, BYTE * &message, DWORD &size)
{
	if(idLetter > this->lettersCount || idLetter < 1) throw Errors(_T("Letter doesn't exist"));
	BOOL b = false;
	DWORD messageSize = 0;
	HANDLE h = CreateFileForRead(this->fileName);
	if (h == IHV) throw Errors(_T("File doesn't exist"));
	SetFilePointer(h, sizeof(this->lettersCount) + sizeof(this->maxSize), 0, FILE_BEGIN); // К началу сообщений
	for (unsigned i = 1; i < idLetter; i++)
	{
		b = ReadFile(h, &messageSize, sizeof(messageSize), &this->dwCount, 0);
		if (!b) throw Errors(_T("File structure is broken"));
		SetFilePointer(h, messageSize, 0, FILE_CURRENT);
	}
	b = ReadFile(h, &messageSize, sizeof(messageSize), &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot read message size"));
	size = messageSize;
	message = (BYTE*)malloc(size);
	b = ReadFile(h, message, size, &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot read message"));
	CloseHandle(h);
	return true;
}

DWORD MailBox::DeleteLetter(DWORD idLetter)
{
	if (idLetter > this->lettersCount) throw Errors(_T("Letter doesn't exist"));
	BOOL b = false;
	DWORD messageSize = 0;
	DWORD prevLetterPointerPos = 0;
	DWORD nextLetterPointerPos = 0;
	
	HANDLE h = CreateFileForReadWrite(this->fileName);
	if (h == IHV) throw Errors(_T("File doesn't exist"));
	DWORD fileSize = GetFileSize(h, 0);

	SetFilePointer(h, sizeof(this->lettersCount) + sizeof(this->maxSize), 0, FILE_BEGIN);
	for (unsigned i = 1; i < idLetter; i++)
	{
		b = ReadFile(h, &messageSize, sizeof(messageSize), &this->dwCount, 0);
		if (!b) throw Errors(_T("File structure is broken"));
		prevLetterPointerPos = SetFilePointer(h, messageSize, 0, FILE_CURRENT);
	}
	b = ReadFile(h, &messageSize, sizeof(messageSize), &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot read message size"));
	nextLetterPointerPos = SetFilePointer(h, messageSize, 0, FILE_CURRENT);

	SetFilePointer(h, 0, 0, FILE_BEGIN);
	BYTE* buf = (BYTE*)calloc(fileSize - (nextLetterPointerPos - prevLetterPointerPos), sizeof(BYTE));
	b = ReadFile(h, buf, prevLetterPointerPos, &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot read file"));
	SetFilePointer(h, nextLetterPointerPos, 0, FILE_BEGIN);
	b = ReadFile(h, buf+prevLetterPointerPos, fileSize - nextLetterPointerPos, &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot read file"));
	CloseHandle(h);

	DeleteFile(this->fileName);
	h = CreateFileForReadWrite(this->fileName);
	if (h == IHV) throw Errors(_T("Cannot create file for result"));
	b = WriteFile(h, buf, fileSize - (nextLetterPointerPos - prevLetterPointerPos), &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot write file for result"));	
	CloseHandle(h);
	free(buf);
	return true;
}
