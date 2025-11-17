#include  <algorithm>
#include <chrono>
#include <execution>
#include <future>
#include <iostream>
#include <thread>  
#include <vector>


using namespace std;

int foo(int value) {
	this_thread::sleep_for(0.5s);
	cout << " Value = " << value << endl;
	return value * 10;
}
int main1()
{
	int a = 15;
	future<int> result = async(foo, a); // Запускает поток, получает значение и возвращает значение, вычтсляется сразу
	//future<int> result = async(launch::deferred, foo, a); // Запускается когда вызывается get
	for (size_t i = 0; i < 10; i++) {
		this_thread::sleep_for(100ms);
		cout << i << " " ;
	} cout <<  endl;
	cout << result.get() << endl;
	
	//-------------------
	this_thread::sleep_for(10s);
	auto s10 = async(
		[]()
		{
			this_thread::sleep_for(10s);
			cout << " Value = 10 " << endl;
		}
	);

	auto s5 = async(
		[]()
		{
			this_thread::sleep_for(5s);
			cout << " Value = 5 " << endl;
		}
	);
	cout << " Value = 1 " << endl;
	return 0;
}
