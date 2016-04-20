// OS_lab_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mailbox.h"
#include "errors.h"
#include <malloc.h>


int main()
{
	try
	{
		printf("-----Mailbox service is started!-----\n\n");

		DWORD mailBoxSize = 200;
		TCHAR* mailbox = L"mailBox1";
		DWORD freeSize = 0;
		if (mailBoxSize == 0)
			_tprintf(_T("Opening mailbox: %s\n"), mailbox);
		else
			_tprintf(_T("Creating mailbox: %s\n"), mailbox);
		MailBox* mb = new MailBox(mailbox, mailBoxSize);

		HANDLE h = OpenFileForWrite(mb->getFileName());
		freeSize = mb->getMaxSize() - GetFileSize(h, 0);
		CloseHandle(h);
		printf("Success! Size: %d/%d\n\n", (mb->getMaxSize() - freeSize), mb->getMaxSize());

		//Add letters
		char* m1 = "THE FIRST LETTER";
		BYTE* b1 = (BYTE*)m1;
		printf("Adding letter #%d: %s\n",mb->getMessageCount() + 1, m1);
		mb->AddLetter(b1, strlen(m1));
		printf("Success!\n\n");

		m1 = "THE SECOND LETTER";
		b1 = (BYTE*)m1;
		printf("Adding letter #%d: %s\n", mb->getMessageCount() + 1, m1);
		mb->AddLetter(b1, strlen(m1));
		printf("Success!\n\n");

		m1 = "THE THIRD LETTER";
		b1 = (BYTE*)m1;
		printf("Adding letter #%d: %s\n", mb->getMessageCount() + 1, m1);
		mb->AddLetter(b1, strlen(m1));
		printf("Success!\n\n");

		m1 = "THE FOURTH LETTER";
		b1 = (BYTE*)m1;
		printf("Adding letter #%d: %s\n", mb->getMessageCount() + 1, m1);
		mb->AddLetter(b1, strlen(m1));
		printf("Success!\n\n");

		m1 = "THE LAST(FIFTH) LETTER";
		b1 = (BYTE*)m1;
		printf("Adding letter #%d: %s\n", mb->getMessageCount() + 1, m1);
		mb->AddLetter(b1, strlen(m1));
		printf("Success!\n\n");

		//ReadLetter
		BYTE* b2 = NULL;
		DWORD messageSize;	
		DWORD idRead = 3;
		printf("Reading letter: #%d\n", idRead);
		mb->ReadLetter(idRead, b2, messageSize);
		char* m2 = (char*)b2;
		printf("Letter: ");
		for (unsigned i = 0; i < messageSize; i++)
			printf("%c", m2[i]);
		printf("\nSize: %d\n", messageSize);
		printf("Success!\n\n");
		free(b2);

		//Delete letter
		/*DWORD idDel = 3;
		printf("Deleting letter: #%d\n", idDel);
		mb->DeleteLetter(idDel);
		printf("Success!\n\n");*/
	}
	catch (Errors e) { }	
	getchar();
    return 0;
}

