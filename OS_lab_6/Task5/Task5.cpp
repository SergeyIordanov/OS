// Task5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

#define LEFT (i+4)%5
#define RIGHT (i+1)%5
#define THINKING 0 // Состояния философов
#define HUNGRY 1
#define EATING 2
#define MIN 100
#define MAX 500

int State[5]; //Массив состояний философов
HANDLE Access; //Управление доступом к критическим данным
HANDLE S[5];//Для ожидания освобождения ресурсов

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
		State[i] = EATING; // Изменяем состояние
		ReleaseSemaphore(S[i], 1, 0); // Разрешаем перейти к еде
	}
}

void take_forks(int i) {
	WaitForSingleObject(Access, INFINITE); // Захватываем доступ к критическим данным
	State[i] = HUNGRY; // Изменяем свое состояние
	test(i); // Пытаемся взять две вилки
	ReleaseSemaphore(Access, 1, 0); // Освобождаем доступ к критическим данным
	WaitForSingleObject(S[i], INFINITE); // Блокируемся, если не удалось взять вилки
	cout << "Philosopher " + std::to_string(i) + " has taken left fork\n\n";
	cout << "Philosopher " + std::to_string(i) + " has taken right fork\n\n";
}
void put_forks(int i) {
	WaitForSingleObject(Access, INFINITE); // Захватываем доступ к критическим данным
	State[i] = THINKING; // Изменяем свое состояние
	cout << "Philosopher " + std::to_string(i) + " has put left fork\n\n";
	test(LEFT); // Пытаемся накормить соседа слева
	cout << "Philosopher " + std::to_string(i) + " has put right fork\n\n";
	test(RIGHT); // Пытаемся накормить соседа справа
	ReleaseSemaphore(Access, 1, 0); // Освобождаем доступ к критическим данным
}

DWORD WINAPI Philosopher(LPVOID param) { // param – номер философа от 0 до 4
	int count = 0;
	int max = 2 + rand() % (4 - 2);
	while (count != max) {
		think(*(int*)param); // Размышляем
		take_forks(*(int*)param); // Берем 2 вилки или блокируемся
		eat(*(int*)param); // Обедаем
		put_forks(*(int*)param); // Кладем на стол обе вилки
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

