#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <queue>
#include <iostream>
#include <functional>
using namespace std;

void print_x(int x)
{
    cout << x << endl;
}

void start_print()
{
    int t = 10;
    thread t1(print_x, t);
    t1.detach();
}

int main()
{
    thread t2(start_print);
    t2.join();
    this_thread::sleep_for(2s);
    return 0;
}