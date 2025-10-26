#define _USE_MATH_DEFINES
#include<iostream>
#include<thread>
#include<mutex>
#include<vector>

using namespace std;
mutex m1;
mutex m2;
mutex m3;
mutex m4;

void deadlock_func_1() //Ошибка владения 2-мя мютекс
{
    cout << "Start deadlock_func_1..." << endl;
    lock_guard<mutex> la{m1};
    this_thread::sleep_for(100ms);
    cout << "deadlock_func_1 acquires mutex 1..." << endl;
    lock_guard<mutex> lb{m2 };

    cout << "Never print A" << endl;
}

void deadlock_func_2() //Ошибка владения 2-мя мютекс
{
    cout << "Start deadlock_func_2..." << endl;
    lock_guard<mutex> lb{m2};
    this_thread::sleep_for(200ms);
    cout << "deadlock_func_2 acquires mutex 2..." << endl;
    lock_guard<mutex> la{ m2 };
    cout << "Never print B" << endl;

}

void deadlock_func_11()
{
    cout << "Start deadlock_func_1..." << endl;
    //scoped_lock L{ m1, m2 }; !! Стандарт 17, только его и все
    //lock(m3, m4); // Атомарный захват обоих мьютексов
    unique_lock<mutex>la{ m3, defer_lock }; // для unique // Не захватывает сразу а в конце look
    this_thread::sleep_for(100ms);
    cout << "deadlock_func_1 acquires mutex 1..." << endl;
    //lock_guard<mutex> lb{ m4, adopt_lock }; //не пытается повторно захватить мьютекс
    unique_lock<mutex>lb{ m4, defer_lock };
    //lock(la,lb)
    cout << "Never print A" << endl;
}

void deadlock_func_22()
{
    cout << "Start deadlock_func_2..." << endl;
    lock(m3, m4); // Атомарный захват обоих мьютексов
    lock_guard<mutex> lb{ m3, adopt_lock };
    this_thread::sleep_for(200ms);
    cout << "deadlock_func_2 acquires mutex 2..." << endl;
    lock_guard<mutex> la{ m4, adopt_lock };
    cout << "Never print B" << endl;

}

int main2()
{
    thread t1(deadlock_func_11);  
    thread t2(deadlock_func_22);
    t1.join();
    t2.join();


    system("Pause");
    return 0;
}