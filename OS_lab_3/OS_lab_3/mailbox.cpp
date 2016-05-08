#include "stdafx.h"
#include "mailbox.h"
#include "errors.h"
#include <malloc.h>

#pragma warning(disable : 4996 )

static const UINT16 CRC_16_TABLE[256] =
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,    /* 00h - 07h */
	0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,    /* 08h - 0Fh */
	0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,    /* 10h - 17h */
	0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,    /* 18h - 1Fh */
	0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,    /* 20h - 27h */
	0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,    /* 28h - 2Fh */
	0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,    /* 30h - 37h */
	0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,    /* 38h - 3Fh */
	0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,    /* 40h - 47h */
	0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,    /* 48h - 4Fh */
	0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,    /* 50h - 57h */
	0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,    /* 58h - 5Fh */
	0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,    /* 60h - 67h */
	0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,    /* 68h - 6Fh */
	0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,    /* 70h - 77h */
	0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,    /* 78h - 7Fh */
	0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,    /* 80h - 87h */
	0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,    /* 88h - 8Fh */
	0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,    /* 90h - 97h */
	0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,    /* 98h - 9Fh */
	0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,    /* A0h - A7h */
	0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,    /* A8h - AFh */
	0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,    /* B0h - B7h */
	0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,    /* B8h - BFh */
	0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,    /* C0h - C7h */
	0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,    /* C8h - CFh */
	0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,    /* D0h - D7h */
	0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,    /* D8h - DFh */
	0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,    /* E0h - E7h */
	0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,    /* E8h - EFh */
	0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,    /* F0h - F7h */
	0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0     /* F8h - FFh */
};

DWORD MailBox::mailBoxesCount = 0;

MailBox::MailBox(TCHAR * fname, DWORD maxSize)
{
	BOOL b = false;
	if (maxSize == 0) {		
		HANDLE h = CreateFileForRead(fname);
		b = h != IHV;
		if (!b) throw Errors(_T("File doesn't exist"));	
		b = this->checkCRC16(fname);
		if (!b) throw Errors(_T("File is broken"));
		SetFilePointer(h, 2, 0, FILE_BEGIN);
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
		UINT16 crc = 0;

		HANDLE h = CreateFileForWrite(fname);
		if (h == IHV) throw Errors(_T("File already exists"));
		this->lettersCount = 0;
		this->maxSize = maxSize;
		this->fileName = fname; //WARNING!!!
		b = WriteFile(h, &crc, 2, &this->dwCount, 0);
		if (!b) throw Errors(_T("Cannot write file"));
		b = WriteFile(h, &this->lettersCount, sizeof(this->lettersCount), &this->dwCount, 0);
		if(!b) throw Errors(_T("Cannot write file"));
		b = WriteFile(h, &maxSize, sizeof(this->maxSize), &this->dwCount, 0);
		if (!b) throw Errors(_T("Cannot write file"));
		CloseHandle(h);
		b = this->setCRC16(fname);
		if (!b) throw Errors(_T("Cannot calc CRC"));
		MailBox::mailBoxesCount++;
	}
}

// структура файла: [controlSum][lettersCount][maxSize][letter1_Size][letter1][letter2_Size][letter2]...

BOOL MailBox::AddLetter(BYTE * message, DWORD size)
{
	BOOL b = this->checkCRC16(this->fileName);
	if (!b) throw Errors(_T("File is broken"));
	HANDLE h = OpenFileForWrite(this->fileName);
	if(h == IHV) throw Errors(_T("File doesn't exist"));
	DWORD fileSize = GetFileSize(h, 0);
	if(fileSize + size + sizeof(DWORD) > maxSize) throw Errors(_T("No more free space in current mailbox"));
	this->lettersCount++;
	SetFilePointer(h, 2, 0, FILE_BEGIN);
	b = WriteFile(h, &lettersCount, sizeof(lettersCount), &this->dwCount, 0);
	if(!b) throw Errors(_T("Cannot write message"));
	SetFilePointer(h, 0, 0, FILE_END);
	b = WriteFile(h, &size, sizeof(size), &this->dwCount, 0);
	if(!b) throw Errors(_T("Cannot write message"));
	b = WriteFile(h, message, size, &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot write message"));
	CloseHandle(h);
	b = this->setCRC16(this->fileName);
	if (!b) throw Errors(_T("Cannot calc CRC"));
	return true;
}

BOOL MailBox::ReadLetter(DWORD idLetter, BYTE * &message, DWORD &size)
{
	BOOL b = this->checkCRC16(this->fileName);
	if (!b) throw Errors(_T("File is broken"));
	if(idLetter > this->lettersCount || idLetter < 1) throw Errors(_T("Letter doesn't exist"));
	DWORD messageSize = 0;
	HANDLE h = CreateFileForRead(this->fileName);
	if (h == IHV) throw Errors(_T("File doesn't exist"));
	SetFilePointer(h, 2 + sizeof(this->lettersCount) + sizeof(this->maxSize), 0, FILE_BEGIN); // К началу сообщений
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
	BOOL b = this->checkCRC16(this->fileName);
	if (!b) throw Errors(_T("File is broken"));
	if (idLetter > this->lettersCount) throw Errors(_T("Letter doesn't exist"));
	DWORD messageSize = 0;
	DWORD prevLetterPointerPos = 0;
	DWORD nextLetterPointerPos = 0;
	
	HANDLE h = CreateFileForReadWrite(this->fileName);
	if (h == IHV) throw Errors(_T("File doesn't exist"));
	DWORD fileSize = GetFileSize(h, 0);

	prevLetterPointerPos = SetFilePointer(h, 2 + sizeof(this->lettersCount) + sizeof(this->maxSize), 0, FILE_BEGIN);
	for (unsigned i = 1; i < idLetter; i++)
	{
		b = ReadFile(h, &messageSize, sizeof(messageSize), &this->dwCount, 0);
		if (!b) throw Errors(_T("File structure is broken"));
		prevLetterPointerPos = SetFilePointer(h, messageSize, 0, FILE_CURRENT);
	}
	b = ReadFile(h, &messageSize, sizeof(messageSize), &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot read message size"));
	nextLetterPointerPos = SetFilePointer(h, messageSize, 0, FILE_CURRENT);

	SetFilePointer(h, 2, 0, FILE_BEGIN);
	this->lettersCount = this->lettersCount - 1;
	b = WriteFile(h, &this->lettersCount, 2, &this->dwCount, 0);
	if (!b) throw Errors(_T("Cannot write number of letters"));

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
	b = this->setCRC16(this->fileName);
	if (!b) throw Errors(_T("Cannot calc CRC"));
	free(buf);
	return true;
}

BOOL MailBox::setCRC16(TCHAR* fname)
{
	BOOL b = false;
	HANDLE h = CreateFileForRead(fname);
	b = h != IHV;
	if (b) {
		DWORD sizeOfData = GetFileSize(h, 0) - 2;
		UINT8 *buf = (UINT8*)calloc(sizeOfData, sizeof(UINT8));
		UINT16 calcCRC = 0;

		SetFilePointer(h, 2, 0, FILE_BEGIN);
		b = ReadFile(h, buf, sizeOfData, &this->dwCount, 0);
		CloseHandle(h);
		if (b) {
			calcCRC = calcCRC16(buf, sizeOfData);

			h = OpenFileForWrite(fname);
			b = h != IHV;
			if (b) {
				b = WriteFile(h, &calcCRC, sizeof(calcCRC), &this->dwCount, 0);
				CloseHandle(h);
			}
		}
		free(buf);
	}
	return(b);
}

BOOL MailBox::checkCRC16(TCHAR* fname)
{
	BOOL b = false;
	HANDLE h = CreateFileForRead(fname);
	b = h != IHV;
	if (b) {
		DWORD sizeOfData = GetFileSize(h, 0) - 2;
		UINT8 *buf = (UINT8*)calloc(sizeOfData, sizeof(UINT8));
		UINT16 readCRC = 0;
		UINT16 calcCRC = 0;

		b = ReadFile(h, &readCRC, sizeof(readCRC), &this->dwCount, 0);
		if (b) {
			b = ReadFile(h, buf, sizeOfData, &this->dwCount, 0);
			if (b) {
				calcCRC = calcCRC16(buf, sizeOfData);
				b = calcCRC == readCRC;
			}
		}
		CloseHandle(h);
		free(buf);
	}
	return(b);
}

UINT16 MailBox::calcCRC16(UINT8 *buf, DWORD sizeOfData)
{
	UINT16 i = 0;
	UINT16 crc16 = 0xFFFF;

	for (i = 0; i < sizeOfData; i++)
	{
		crc16 = (crc16 << 8) ^ CRC_16_TABLE[(UINT8)(crc16 >> 8) ^ buf[i]];
	}

	return(crc16);
}


void MailBox::ReadUsingFileMapping() {
	HANDLE h = CreateFileForReadWrite(this->fileName);
	if (h == IHV) throw Errors(_T("File doesn't exist"));
	DWORD fileSize = GetFileSize(h, 0);
	HANDLE hFileMapping; // объект ядра "проекция файла"
	hFileMapping = CreateFileMapping(h, 0, PAGE_READWRITE, 0, fileSize, (LPCWSTR)"map1");
	CloseHandle(h);
	if (hFileMapping == 0){
		printf("Cannot create file mapping");
		return;
	}
	//PBYTE pbFile; // массив байт всего файла
	BYTE* pbFile;
	// Проецируем в адресное пространство процесса объект "проекция файла"
	pbFile = (BYTE*)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	CloseHandle(hFileMapping);
	if (pbFile == 0) {
		printf("Cannot map file to the memory");
		return;
	}

	//printf("%d", );
	printf("\n");
	char* m2 = (char*)pbFile;
	int letterSize = 0;
	int count = 1;
	for (unsigned i = 10; i < fileSize; count++) {
		letterSize = (int)m2[i];
		int stop = letterSize + i + 4;
		printf("Letter %d: ", count);
		for (i += 4; i < stop; i++) {
			printf("%c", (int)m2[i]);
		}
		printf("\n");
	}
	if (!UnmapViewOfFile(pbFile)) {
		printf("Cannot unmap memory!\nRestart the program");
		return;
	}
	printf("\n");
}
