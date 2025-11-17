#include <iostream>
#include <semaphore>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <barrier>
#include <latch>
#include "Header.h"


// Пример синхранизации



using namespace std;
mutex m_latch;
//latch L{ 3 }; //объект для ожидания потоками, кол потоков
barrier B{ 3 }; //возможно переинициировать

void two_stage(int value, int id)
{
    for (int i = value; i > 0; i--) {
        this_thread::sleep_for(200ms * id);
        lock_guard < mutex> l(m_latch);
        cout << "поток" << id << " вниз " << i << endl;
    }
   // L.count_down(); //уменьшаем счетчик
    m_latch.lock();
    consol_color::SetColor(id + 2, 0);
    cout << "поток" << id << " pause "  << endl;
    consol_color::SetColor(15, 0);
    m_latch.unlock();
    B.arrive_and_wait();
    //L.wait(); // не будет идти пока не равен будет 0

    for (int i = 0; i < value; i++) {
        this_thread::sleep_for(200ms * id);
        lock_guard < mutex> l(m_latch);
        cout << "поток" << id << " вверх " << i << endl;
    }
}

int main3()
{
    setlocale(LC_ALL, "Rus");
    /*
    
    two_stage(5, 1);
    system("Pause");
    */

    vector<jthread> Vt;
    for (size_t i = 0; i < 3; i++) {
        Vt.push_back(jthread(two_stage, 5, i + 1)); // сейчас все работают в разнобой 
    }
    return 0;
}
