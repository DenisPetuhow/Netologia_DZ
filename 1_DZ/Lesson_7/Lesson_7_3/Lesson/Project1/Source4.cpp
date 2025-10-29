#include  <algorithm>
#include <chrono>
#include <execution>
#include <future>
#include <iostream>
#include <thread>  
#include <vector>
#include <random>
#include <numeric>
#include "Timer.h"


using namespace std;

namespace
{
	template<typename Iter, typename T>
	T parallel(Iter first, Iter last, T init) {
		size_t size = distance(first, last);
		if (size < 10) {
			return std::accumulate(first, last, init); // если мало переменных выполняем в едином потоке 
		}
		else {
			Iter middle = first;
			advance(middle, size / 2); // сдвигаем переменную на середину 
			auto Res_1 = async(parallel<Iter, T>, first, middle, init); //запускается параллельно
			T Res_2 = parallel<Iter, T>(middle, last, init);
			return Res_1.get() + Res_2;
		}
	}
	
}
int main()
{
	vector<int> V1(100, 3);
	Timer T("par");
	cout << parallel(V1.begin(), V1.end(), 0) << endl;
	T.print();

	return 0;
}