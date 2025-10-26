#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <queue>
#include <iostream>
using namespace std;

bool flag = false;
mutex m10;

void wait_flag()
{
    unique_lock<mutex> l(m10);
    while (!flag)  // Пока фалс сначала берет мютекс а потом освобождает, для 2 потока
    {
        l.unlock();
        this_thread::sleep_for(200ms);
        l.lock();
        cout << "Flag not set" << endl;
    }
    cout << "Flag was set!!!" << endl;
}

void set_flag()
{
    this_thread::sleep_for(1s);
    unique_lock<mutex> l(m10);
    cout << "Flag UPPPP was set!!!" << endl;
    flag = true; // Важно запись во флаг под мютексом
    l.unlock();
}

//---------------Синхронизация через условные переменные

mutex mut;
queue<int> positive_data_queue;
queue<int> negative_ata_queue;
condition_variable notice;

void dataGenerator()
{
    int cnt = 1;
    while (cnt != 0)
    {
        this_thread::sleep_for(10ms);
        cnt = rand() % 30 - 15;
        std::unique_lock<mutex> lk(mut);
        if (cnt < 0) {
            negative_ata_queue.push(cnt);
            cout << "PUSH___"  << cnt<< endl;
        }
        else if (cnt > 0) {
            positive_data_queue.push(cnt);
            cout << "PUSH___" << cnt << endl;
        } else { 
            cout << "ENNDDDD" << endl; 

            // Без этих строк потоки positive() и negative() 
            // останутся заблокированными в wait()
            negative_ata_queue.push(cnt);
            positive_data_queue.push(cnt);
        }
        notice.notify_all();
    }
}

void positive() // работает пока не ноль, как ноль он не запустит whike и закончит поток
{
    int cnt = 1;
    while (cnt != 0)
    {
        std::unique_lock<std::mutex> lk(mut);
        notice.wait(lk, []() { return !positive_data_queue.empty(); } );  //Обертка для просыпанияб просыпатся как срабатывает уведомления, функтор для проверки
        
        cnt = positive_data_queue.front();
        positive_data_queue.pop();
        cout << "POP FROM positive - " << cnt << endl;
        lk.unlock();
    }
}

void negative()
{
    int cnt = 1;
    while (cnt != 0)
    {
        std::unique_lock<std::mutex> lk(mut);
        notice.wait(lk, []() { return !negative_ata_queue.empty(); });  //Обертка для просыпанияб просыпатся как срабатывает уведомления, функтор для проверки
     
        cnt = negative_ata_queue.front();
        negative_ata_queue.pop();
        cout << "POP FROM positive - " << cnt << endl;
        lk.unlock();
    }
}

//-- ЕЩЕ ПРИМЕР 

mutex mut1;
queue<int> data_queue;
condition_variable data_cond;

void data_preparation_thread()
{
    int cnt = 0;
    while (cnt < 10)
    {
        int const data = rand() % 100;
        std::lock_guard<std::mutex> lk(mut1);
        this_thread::sleep_for(10ms);
        data_queue.push(data);
        cout << "Push " << data << endl;
        data_cond.notify_all();
        if (data == 0)
            break;
    }
}

void data_processing_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut1);
        data_cond.wait(lk, [] {return !data_queue.empty(); });
        int  data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        cout << "  Pop " << data << endl;
        if (data == 0)
            break;
    }
}   

int main3()
{

    thread t3(dataGenerator);
    thread t4(positive);
    thread t5(negative);
    t3.join();
    t4.join();
    t5.join();


    thread t1(data_preparation_thread);
    thread t2(data_processing_thread);
    t1.join();
    t2.join();
    return 0;
}