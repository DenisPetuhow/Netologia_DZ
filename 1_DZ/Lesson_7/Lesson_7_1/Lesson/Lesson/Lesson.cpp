

#include <iostream>
#include <vector>
#include <random>
#include <execution>
#include <algorithm>
#include <chrono>
#include<thread>
#include "Timer.h"

using namespace std;

void create_random(std::vector <int>& V) // создание рандомного числа
{
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist(0, 100000);
    auto random_num([=]() mutable {return dist(gen); });
    std::generate(V.begin(), V.end(), random_num);

	//std::mt19937 — тип генератора псевдослучайных чисел(алгоритм «Вихрь Мерсенна»).Это один из наиболее популярных генераторов, обеспечивающих высокое качество случайности.
		//gen — экземпляр генератора.Важно: он не инициализирован явно(например, с помощью std::random_device), поэтому при каждом запуске программы будет генерировать одну и ту же последовательность чисел.Это основная проблема данной реализации.
	//std::uniform_int_distribution<int> — равномерное распределение целых чисел в заданном диапазоне.
	//[=] — захват переменных из внешней области видимости по значению(копируются gen и dist).Это означает, что внутри лямбды будут использоваться копии генератора и распределения.
		//mutable — позволяет изменять захваченные копии внутри лямбды(без этого ключевого слова скопированные объекты gen и dist считались бы константными).
	//{ return dist(gen); } — тело лямбды : при каждом вызове возвращает новое случайное число, используя скопированные gen и dist.
}

void sort(std::vector <int>& v)
{
    auto start = std::chrono::steady_clock::now();
    sort(v.begin(), v.end());
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration <double, std::ratio<1, 1>> time = end - start;
    std::cout << time.count() << std::endl;
}

//---------- Вывод времени расчета через chrono

void func1()
{
	cout << "#id = " << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(100ms);
		console_color::SetColor(15, 0);
		cout << i << " ";
	}
	cout << endl;
}

void func2()
{
	cout << "#id = " << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(100ms);  //пауза в печати
		console_color::SetColor(10, 0);
		cout << i << " ";
	}
	cout << endl;
}

class A
{
public:
	thread t;
	A()
	{
		t = thread(&A::funcA, this); // создаем поток через конструктор объекта
		cout << "THUS 2" << endl;
	}
	~A() {
		t.join(); // ждать до завершения
	}
	void funcA()
	{
		cout << __FUNCTION__ << endl;
	}

	static void funcAST()
	{
		cout << __FUNCTION__ << endl;
	}
	void operator()()
	{
		cout << "Functor" << endl;
	}
};


int main1()
{
    std::vector<int> V1(1000000);
    create_random(V1);
    sort(V1);
    //----------

	cout << "#id main = " << this_thread::get_id() << endl;
	cout << thread::hardware_concurrency() << endl;
	thread t1(func1);
	thread t2(func2);
	if (t1.joinable())
	{
		cout << "t1.joinable()" << endl;
	}
	t1.join(); //ждет основной поток, когда дочерние потоки закончат
	
	t2.join(); //detach не ждет
	system("pause"); 

	//-----

	A obj;
	thread t3(&A::funcA, &obj); //вызов метода с передачей объекта
	thread t4(&A::funcAST); // без создания объекта
	t4.join();
	thread t5(&A::operator(), &obj);
	t5.join();

	thread t6([]() {cout << __FUNCTION__ << endl; });
	t6.join();


    return 0;
}

