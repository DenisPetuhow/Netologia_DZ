/*#include<thread>
#include<mutex>
#include "Timer.h"

using namespace std;

class some_data
{
	int a;
	std::string b;
public:
	void do_something() {};
};

class data_wrapper
{
private:
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard<std::mutex> l(m);
		func(data);
	}
};

some_data* unprotected;
void malicious_function(some_data& protected_data)
{
	unprotected = &protected_data;
}


data_wrapper x;
void foo()
{
	x.process_data(malicious_function);
	unprotected->do_something();
}

int main()
{


	return 0;
}

*/

#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <queue>
#include <iostream>
#include <atomic>

using namespace std;

mutex mtx;
atomic_flag work_flag = ATOMIC_FLAG_INIT; // флаг создан **сброшенным** (clear)

void wait_flag22() 
{
	while (!work_flag.test()) // // test() возвращает текущее состояние, тоесть false
	{
		this_thread::sleep_for(100ms);
		
		std::cout << "Not   SETTT "<< endl;
	}
	std::cout << "   SETTT " << endl;
}


void set_flag22()
{

	this_thread::sleep_for(1000ms);
	work_flag.test_and_set(); // устанавливает флаг в 1
	std::cout << "Seting Flag " << endl;

}

atomic<int> x{ 0 };
atomic<int> y{ 0 };

void f1() {
	x.store(10, memory_order_seq_cst);
	int res = y.load();
	this_thread::sleep_for(100ms);
	std::cout << "   f1 " << res << endl;
}

void f2() {
	y.store(100, memory_order_seq_cst);
	int res = x.load();
	this_thread::sleep_for(100ms);
	std::cout << "   f2 " << res << endl;
}

int main() {

	thread t1(wait_flag22);
	thread t2(set_flag22);
	t1.join();
	t2.join();

	std::cout << boolalpha << x.is_lock_free() << endl;
	thread t3(f1);
	thread t4(f2);
	t3.join();
	t4.join();


	return 0;
}