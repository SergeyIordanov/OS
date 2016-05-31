// Task3(4).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

unsigned readcount;
HANDLE countsem, wsem, rsem;
std::vector<std::string> news;
long temp = 0;

DWORD WINAPI reader(LPVOID parametr) {
	while (news.size() == 0) {}
		std::string name((char*)parametr);
		WaitForSingleObject(countsem, INFINITE);
		readcount++;
		if (readcount == 1) {
			WaitForSingleObject(wsem, INFINITE);
		}
		if (readcount > 0) {	
			//cout << "start if(readcount > 0)" + name + "\n\n";
			ReleaseSemaphore(countsem, 1, 0);
					
			std::string forPrint = name + " read issue_" + std::to_string(news.size()) + "\tCurrent readers: " + std::to_string(readcount) + "\n\n";
			cout << forPrint;
			//cout << "before rsem release " + name + "\n\n";
			WaitForSingleObject(countsem, INFINITE);
			//cout << "after rsem release " + name + "\n\n";
			readcount--;
			//cout << "end if(readcount > 0)" + name + "\n\n";
		}
		if (readcount == 0) {
			//cout << "in if(readcount == 0)" + name + "\n\n";		
			ReleaseSemaphore(wsem, 1, &temp);	
			cout << "Data access released\n\n";
			//cout << "end if(readcount == 0)" + name + "\n\n";
		}
		//cout << "count released" + name + "\n\n";
		Sleep(10);
		ReleaseSemaphore(countsem, 1, 0);
	//}
	return 0;
}

DWORD WINAPI writer(LPVOID parametr) {
	std::string name((char*)parametr);
	WaitForSingleObject(wsem, INFINITE);
	std::string issue = name + ": issue_" + std::to_string(news.size());
	news.push_back(issue);
	std::string forPrint = name + " pushed issue_" + std::to_string(news.size()) + "\n\n";
	cout << forPrint;
	//cout << "writer here!" << endl;
	ReleaseSemaphore(wsem, 1, &temp);
	return 0;
}

LPVOID copy(std::string s) {
	int size = s.size();
	char* temp = new CHAR[size + 1];
	memcpy_s(temp, size, (char*)s.c_str(), size);
	temp[size] = '\0';
	return temp;
}

int _tmain(int argc, _TCHAR* argv[])
{
	readcount = 0;
	countsem = CreateSemaphore(0, 1, 1, _T("reader"));
	wsem = CreateSemaphore(0, 1, 1, _T("writer"));
	rsem = CreateSemaphore(0, 0, 1, _T("nuller"));
	

	const int readerLength = 15;
	const int writerLength = 5;
	printf("Numer of readers: %d\nNumber of writers: %d\n\n", readerLength, writerLength);
	HANDLE threadHandles[readerLength + writerLength];
	for (int i = 0; i < readerLength; i++)
	{
		std::string readerName = "Reader_" + std::to_string(i);
		threadHandles[i] = CreateThread(NULL, 0, reader, copy(readerName), 0, NULL);
	}
	for (int i = 0; i < writerLength; i++)
	{
		std::string writerName = "Writer_" + std::to_string(i);
		threadHandles[i + readerLength] = CreateThread(NULL, 0, writer, copy(writerName), 0, NULL);
	}
	WaitForMultipleObjects(writerLength + readerLength, threadHandles, TRUE, INFINITE);
	for (int i = 0; i < readerLength + writerLength; i++)
		CloseHandle(threadHandles[i]);
	CloseHandle(countsem);
	CloseHandle(wsem);
	CloseHandle(rsem);
	return 0;
}

