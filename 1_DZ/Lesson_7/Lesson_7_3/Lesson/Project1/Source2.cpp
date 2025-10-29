#include  <algorithm>
#include <chrono>
#include <execution>
#include <future>
#include <iostream>
#include <thread>  
#include <vector>
#include <random>

// Передаем фиче в потоки, а выполнение основноо потока делаем через промис
using namespace std;

namespace
{
	
	void wait_pr(shared_future<int>& Future) { //Запускаем wait в отдельный поток, потом срабатывает
		shared_future<int> local_Future = Future; //лучше делать через копирование, так как основной фючер во владении 2 потоков
		Future.wait();
		cout << "Future " << local_Future.get() << endl;
	}

	void wait_pr2(shared_future<int>& Future) { //Запускаем wait в отдельный поток, потом срабатывает
		shared_future<int> local_Future = Future;
		Future.wait();
		cout << "Future2 " << local_Future.get() << endl;
	}
}
int main3()
{
	

	promise<int> Promise2;
	// future<int> Future2 = Promise2.get_future(); // ошибка так как один фьче в два потока ион не может разделится
	shared_future<int> Future2 = Promise2.get_future();
	thread t1(wait_pr, ref(Future2));
	thread t2(wait_pr2, ref(Future2));
	int value2 = 0;
	do {
		cin >> value2;
		if (value2 == 15) {
			Promise2.set_value(value2);
		}
	} while (value2 != 15);
	t1.join();
	t2.join();

	return 0;
}