// OS_lab_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mailbox.h"
#include "errors.h"
#include <malloc.h>
#include <iostream>

using namespace std;

//#pragma warning(disable : 4996 )

int main()
{

		printf("-----Mailbox service is started!-----\n");
		printf("r - restart;\nq - exit.\n");

		MailBox* mb = 0;

		while (true)
		{
			BOOL exit = false;
			try
			{
				printf("\n1 Create new mailbox \n2 Open existed\n");
				char ch1;
				cin >> ch1;
				if (ch1 == '1')
				{
					char mailbox[100];
					DWORD mailBoxSize = 0;
					DWORD freeSize = 0;
					cout << "Write mailbox title:\n";
					cin >> mailbox;
					cout << "Enter size:\n";
					cin >> mailBoxSize;
					TCHAR mailboxTCHAR[100];
					for (int i = 0; i < 100; i++)
						mailboxTCHAR[i] = (TCHAR)mailbox[i];

					mb = new MailBox(mailboxTCHAR, mailBoxSize);

					HANDLE h = OpenFileForWrite(mb->getFileName());
					freeSize = mb->getMaxSize() - GetFileSize(h, 0);
					CloseHandle(h);
					printf("Success! Size: %d/%d\n\n", (mb->getMaxSize() - freeSize), mb->getMaxSize());
				}
				else if (ch1 == '2')
				{
					char mailbox[100];
					DWORD mailBoxSize = 0;
					DWORD freeSize = 0;
					cout << "Write mailbox title:\n";
					cin >> mailbox;
					TCHAR mailboxTCHAR[100];
					for (int i = 0; i < 100; i++)
						mailboxTCHAR[i] = (TCHAR)mailbox[i];

					mb = new MailBox(mailboxTCHAR, mailBoxSize);

					HANDLE h = OpenFileForWrite(mb->getFileName());
					freeSize = mb->getMaxSize() - GetFileSize(h, 0);
					CloseHandle(h);
					printf("Success! Size: %d/%d\n\n", (mb->getMaxSize() - freeSize), mb->getMaxSize());
				}
				else if (ch1 == 'r')
				{
					continue;
				}
				else if (ch1 == 'q')
				{
					char ch2;
					cout << "Exiting. Are you sure? (y/n)";
					cin >> ch2;
					if(ch2 == 'y')
						break;
				}
				else
				{
					printf("Wrong input!\n\n");
					continue;
				}

				while (true)
				{
					try {
						printf("\n1 Add letter\n2 Read letter\n3 Delete letter\n4 Read all messages (using file mapping)\nq - exit\nr - restart\n");
						char ch3;
						cin >> ch3;
						if (ch3 == '1')
						{
							char m1[1000];
							cout << "Write message:\n";
							cin >> m1;
							BYTE* b1 = (BYTE*)m1;
							printf("Adding letter #%d: %s\n", mb->getMessageCount() + 1, m1);
							mb->AddLetter(b1, strlen(m1));
							printf("Success!\n\n");
						}
						else if (ch3 == '2')
						{
							BYTE* b2 = NULL;
							DWORD messageSize;
							DWORD idRead = 0;
							cout << "Enter letter id:\n";
							cin >> idRead;
							printf("Reading letter: #%d\n", idRead);
							mb->ReadLetter(idRead, b2, messageSize);
							char* m2 = (char*)b2;
							printf("Letter: ");
							for (unsigned i = 0; i < messageSize; i++)
								printf("%c", m2[i]);
							printf("\nSize: %d\n", messageSize);
							printf("Success!\n\n");
							free(b2);
						}
						else if (ch3 == '3')
						{
							DWORD idDel;
							cout << "Enter letter id:\n";
							cin >> idDel;
							printf("Deleting letter: #%d\n", idDel);
							mb->DeleteLetter(idDel);
							printf("Success!\n\n");
						}
						else if (ch3 == '4')
						{
							mb->ReadUsingFileMapping();
						}
						else if (ch3 == 'r')
						{
							break;
						}
						else if (ch3 == 'q')
						{
							char ch4;
							cout << "Exiting. Are you sure? (y/n)";
							cin >> ch4;
							if (ch4 == 'y')
							{
								exit = true;
								break;
							}
						}
						else
						{
							printf("Wrong input!\n\n");
							continue;
						}
					}
					catch (Errors e) { getchar(); printf("\n");
					}
				}
				if (exit)
					break;
			}
			catch (Errors e) { getchar(); }
			getchar();
		}

		/*DWORD mailBoxSize = 200;
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
		printf("Success! Size: %d/%d\n\n", (mb->getMaxSize() - freeSize), mb->getMaxSize());*/

		//Add letters
		/*char* m1 = "THE FIRST LETTER";
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
		printf("Success!\n\n");*/

		//ReadLetter
		/*BYTE* b2 = NULL;
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
		free(b2);*/

		//Delete letter
		/*DWORD idDel = 3;
		printf("Deleting letter: #%d\n", idDel);
		mb->DeleteLetter(idDel);
		printf("Success!\n\n");*/
    return 0;
}

