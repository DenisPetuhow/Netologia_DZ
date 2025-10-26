#define _USE_MATH_DEFINES
#include<iostream>
#include<thread>
#include<mutex>
#include "Timer.h"

using namespace std;

mutex m; //объект внутри которой буливска€ атомарна€ переменна€(2 потока не могут получить к ней доступ) 

void func1()
{
	lock_guard<mutex> grd(m);
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(100ms);
		consol_color::SetColor(15, 0);
		cout << i << " ";
	}
	cout << endl;
}

void func2()
{
	m.lock(); //
	//lock_guard<mutex> grd(m);
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(100ms);
		consol_color::SetColor(10, 0);
		cout << i << " ";
	}
	cout << endl;
	consol_color::SetColor(15, 0);
	m.unlock();
}

void func3()
{
	
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(100ms);
		lock_guard<mutex> grd(m);
		consol_color::SetColor(15, 0);
		cout << i << " ";
	}
	cout << endl;
}

void func4()
{
	
	//lock_guard<mutex> grd(m);
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(100ms);
		m.lock();
		consol_color::SetColor(10, 0);
		cout << i << " ";
		m.unlock();
	}
	cout << endl;
	consol_color::SetColor(15, 0);
	
}

recursive_mutex m11;
void rec_print(int x)
{
	m11.lock();
	cout << x << " ";
	this_thread::sleep_for(100ms);
	if (x == 0)
	{
		cout << endl;
		m11.unlock();
		return;
	}
	rec_print(--x);
	m11.unlock();
}

void rec_print1(int x)
{
	lock_guard<recursive_mutex> lk(m11);  //применение умного мютекса в рекурсии
	cout << x << " ";
	this_thread::sleep_for(100ms);
	if (x == 0)
	{
		cout << endl;

		return;
	}
	rec_print(--x);
	
}




int main1()
{
	thread t1(func1);
	thread t2(func2);
	t1.join();
	t2.join();

	//----«асунул mutex в цыкл for и подругому выводит
	thread t3(func3);
	thread t4(func4);
	t3.join();
	t4.join();

	thread t5(rec_print, 6);
	thread t6(rec_print, 4);
	t5.join();
	t6.join();

	thread t7(rec_print1, 6);
	thread t8(rec_print1, 4);
	t7.join();
	t8.join();


	return 0;
}
