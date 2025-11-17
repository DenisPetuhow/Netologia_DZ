#include<iostream>
#include<thread>
#include<vector>
#include<string>
#include<mutex>

using namespace std;
thread_local int x = 2;


void func3(int x, string str)
{
	str += "def";
	cout << x << " " << str << endl;
}

int func4(int x)
{
	return x * 4;
}

void func5()
{
	this_thread::sleep_for(100ms);
	x = x * 10;
	cout << x << endl;
}
void func6()
{
	this_thread::sleep_for(500ms);
	x = x + 10;
	cout << x << endl;
}

//----------Создание сразу потоков из массива

once_flag flag; //проверяется потоками аттамарно, когда один меняет, другие его не видят

void print_once()
{
	cout << "!";
}

void print(size_t x)
{
	call_once(flag, print_once); // врезультате запускает только один поток, для других эта команда занята
	cout << x << " ";
}

int main2()
{
	int v = 3;
	string s = "abs";
	thread t1(func3, v, s); //параметры передаются по значению
	t1.join();
	cout << s << endl;

	int res; //вывод переменной из потока
	thread t2([&]() {res = func4(20); });
	t2.join();
	cout << res << endl;


	thread t3(func6);
	thread t4(func5);
	t3.join();
	t4.join();

	//------------- 

	vector<thread> TV;
	for (size_t i = 0; i < 10; i++)
	{
		TV.push_back(thread(print, i)); //создание 10 потоков
	}
	for (auto& t : TV)
	{
		t.join();
	}
	cout << endl;
	return 0;
}
