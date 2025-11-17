#include <iostream>
#include <semaphore>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <barrier>
#include <latch>



// Пример работы по очереди, один извлекать а другой обрабатывает, ждут друг друга



using namespace std;
int value = 0;
bool flag = false;
binary_semaphore s1{ 0 };
binary_semaphore s2{ 0 };


void Acquisition()
{
    while (!flag) {
        this_thread::sleep_for(0.5s);
        value = rand() % 10;
        cout << "new value " << value  << endl;
        s1.release(); //шоворим что пер поок завершил задачу
        s2.acquire();
    }
}

void Process()
{
    int sum = 0;
    while (!flag) {
        s1.acquire(); //пока флаг не опустится командой s1.release();
        sum += value;
        cout << "sum " << sum << endl;
        if (sum > 50) flag = true;
        s2.release();
    }
}

int main4()
{
    setlocale(LC_ALL, "Rus");
    jthread t1(Acquisition);
    jthread t2(Process);
    return 0;
}
