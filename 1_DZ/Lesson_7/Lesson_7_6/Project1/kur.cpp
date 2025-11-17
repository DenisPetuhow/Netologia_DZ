#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <queue>

using namespace std;

vector <jthread> pool;
queue<function<void()>>  tasks;
mutex m;
bool stop_flag = false;


void F1()
{
    cout << "F1 " << endl;
}

void F2()
{
    cout << "F2 " << endl;
}

void work() {

    m.lock();
    cout << "id : " << this_thread::get_id() << endl;
    m.unlock();
    while (!stop_flag) {
        if (!tasks.empty()) { // если очередь не пустая
            auto task = tasks.front();
            tasks.pop();
            task();
            //this_thread::sleep_for(100ms);
        } 
        else {
            this_thread::yield();
        }
    }
}

//В целом, эта структура часто используется для реализации пула потоков: задачи добавляются в очередь, а потоки извлекают и выполняют их. Вектор потоков (`pool`) управляет рабочими потоками, 
// в то время как очередь задач (`tasks`) содержит работу, которая должна быть выполнена.


 

int main()
{
    setlocale(LC_ALL, "Rus");

    const int size_thr = thread::hardware_concurrency() - 1; // равной количеству аппаратных потоков (ядер) процессора, за вычетом одного.  один часто используется для оптимизации многонишевой обработки, чтобы оставить один поток для системных задач или снизить нагрузку.
    for (size_t i = 0; i < size_thr; i++) {
        pool.push_back(jthread(work));
    }
    
    for (size_t i = 0; i < 10; i++) {
        this_thread::sleep_for(100ms);
        tasks.push(F1);
        tasks.push(F2);
    }




    return 0;
}
