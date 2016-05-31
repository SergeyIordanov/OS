// Task5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

#define LEFT (i+4)%5
#define RIGHT (i+1)%5
#define THINKING 0 // ��������� ���������
#define HUNGRY 1
#define EATING 2
#define MIN 100
#define MAX 500

int State[5]; //������ ��������� ���������
HANDLE Access; //���������� �������� � ����������� ������
HANDLE S[5];//��� �������� ������������ ��������

void wait()
{
	Sleep(MIN + rand() % (MAX - MIN));
}

void think(int i) {
	std::string forPrint = "Philosopher " + std::to_string(i) + " is thinking.." + "\n\n";
	cout << forPrint;
	wait();
}

void eat(int i) {
	std::string forPrint = "Philosopher " + std::to_string(i) + " is eating.." + "\n\n";
	cout << forPrint;
	wait();
}

void test(int i) {
	if (State[i] == HUNGRY && State[LEFT] != EATING && State[RIGHT] != EATING) {
		State[i] = EATING; // �������� ���������
		ReleaseSemaphore(S[i], 1, 0); // ��������� ������� � ���
	}
}

void take_forks(int i) {
	WaitForSingleObject(Access, INFINITE); // ����������� ������ � ����������� ������
	State[i] = HUNGRY; // �������� ���� ���������
	test(i); // �������� ����� ��� �����
	ReleaseSemaphore(Access, 1, 0); // ����������� ������ � ����������� ������
	WaitForSingleObject(S[i], INFINITE); // �����������, ���� �� ������� ����� �����
	cout << "Philosopher " + std::to_string(i) + " has taken left fork\n\n";
	cout << "Philosopher " + std::to_string(i) + " has taken right fork\n\n";
}
void put_forks(int i) {
	WaitForSingleObject(Access, INFINITE); // ����������� ������ � ����������� ������
	State[i] = THINKING; // �������� ���� ���������
	cout << "Philosopher " + std::to_string(i) + " has put left fork\n\n";
	test(LEFT); // �������� ��������� ������ �����
	cout << "Philosopher " + std::to_string(i) + " has put right fork\n\n";
	test(RIGHT); // �������� ��������� ������ ������
	ReleaseSemaphore(Access, 1, 0); // ����������� ������ � ����������� ������
}

DWORD WINAPI Philosopher(LPVOID param) { // param � ����� �������� �� 0 �� 4
	int count = 0;
	int max = 2 + rand() % (4 - 2);
	while (count != max) {
		think(*(int*)param); // ����������
		take_forks(*(int*)param); // ����� 2 ����� ��� �����������
		eat(*(int*)param); // �������
		put_forks(*(int*)param); // ������ �� ���� ��� �����
		count++;
	}
	return 0;
}

int main()
{
	int numbers[] = { 0, 1, 2, 3, 4 };
	Access = CreateSemaphore(0, 1, 1, L"access");
	for (unsigned i = 0; i < 5; i++)
		S[i] = CreateSemaphore(0, 0, 1, 0);
	HANDLE phils[5];
	for (unsigned i = 0; i < 5; i++) {
		phils[i] = CreateThread(0, 0, Philosopher, &numbers[i], 0, 0);
	}
	WaitForMultipleObjects(5, phils, TRUE, INFINITE);
	for (unsigned i = 0; i < 5; i++)
		CloseHandle(phils[i]);
    return 0;
}

