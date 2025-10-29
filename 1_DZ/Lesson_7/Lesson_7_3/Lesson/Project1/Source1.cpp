#include  <algorithm>
#include <chrono>
#include <execution>
#include <future>
#include <iostream>
#include <thread>  
#include <vector>
#include <random>


using namespace std;

namespace
{
	int foo(int value) {
		this_thread::sleep_for(0.5s);
		cout << " Value = " << value << endl;
		return value * 10;
	}

	int get_number(int val, promise<int> Promise) { // уШЕЛ ПРОМИС А ФЬЮЧЕ ОСТАЛСЯ В ОСНОВНОМ ПОТОКЕ
		random_device rd;
		mt19937 gen{ rd() };
		uniform_int_distribution <> rnd{ 0, 30 };
		int value = 0; 
		do {
			value = rnd(gen);
			cout << value << " ";
			this_thread::sleep_for(100ms);

		} while (value != val);
		Promise.set_value(value);
		return 100;
	}

	void wait_pr(future<int>& Future) { //Запускаем wait в отдельный поток, потом срабатывает
		Future.wait();
		cout << "Future " << Future.get() << endl;
		}
}
int main2()
{
	packaged_task<int(int)> task(foo); // вЫЗЫВАЕМ функцию 
	future<int> result = task.get_future(); // присваевываем значение переменной в будущем
	thread t1(move(task), 100);
	t1.join();
	//auto res = async(move(task), 100); можно не преветствуется, и результат хранится в result а не res
	cout <<  result.get()  << endl;


	//// Promise

	promise<int> Promise;
	future<int> Future = Promise.get_future(); // соеденяем с промис
	auto res = async(get_number, 15, move(Promise)); //вызываем функцию ипередаем промис, //или через threade но нельзя вывести res.get()
	Future.wait(); // оЖИДАНИЕ, КОГДА ЗАПИСАН ПОЙДЕТ ДАЛЬШЕ
	cout << "Future " << Future.get() << endl;
	cout << "res " << res.get() << endl;
	cout  << endl;

	promise<int> Promise2;
	future<int> Future2 = Promise2.get_future();
	thread t3(wait_pr, ref(Future2));
	int value2 = 0;
	do {
		cin >> value2;
		if (value2 == 15) {
			Promise2.set_value(value2);
		}
	} while (value2 != 15);
	t3.join();

	return 0;
}