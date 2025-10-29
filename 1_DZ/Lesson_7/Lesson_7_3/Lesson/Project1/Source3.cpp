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

	double DIV(double a, double b) {
		if (b == 0) {
			throw logic_error("zero");
		}
		else {
			return a / b;
		}
	}

	void except_promise(promise<int> Promise, int var) {
		if (var == 0) {
			Promise.set_value(11);
		}
		else {
			try {
				throw "exception";

			}
			catch (const char* excp) {
				Promise.set_exception(current_exception()); // поймали исключение и передали через промис
				//Promise.set_exception(make_exception_ptr(logic_error("new exception"))); // чтобы обрабатывать обычные исключения
			}
		}
	}
}
int main4()
{
	try {
		future<double> Future = async(DIV, 1, 0);
		cout << Future.get() << endl;
	}

	catch (const std::exception& err) {
		cout << err.what() << endl;
	}


	promise<int> Promise;
	future<int> Future = Promise.get_future();
	thread t1(except_promise, move(Promise), 1);
	Future.wait();
	try {
		cout << Future.get() << endl;
	}

	catch (const char* excp) {
		cout << excp << endl; // вывели excp, котрое также может хранить текс при исключении
	}

	t1.join();


	return 0;
}