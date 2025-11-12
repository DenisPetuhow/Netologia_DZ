#include <iostream>
#include <semaphore>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
mutex m1;


//void foo1() {
//    cout << "star threade  " << this_thread::get_id() << endl;
//    system("Pause");
//    cout << "stop threade  " << this_thread::get_id() << endl;
//}

//void foo2() {
//    cout << "star threade  " << this_thread::get_id() << endl;
//    thread t1(foo1);
//    this_thread::sleep_for(std::chrono::milliseconds(500));
//    t1.join();
//    cout << "stop threade  " << this_thread::get_id() << endl;

//}
//----------------------------
void foo1() {
    cout << "star threade  " << this_thread::get_id() << endl;
    system("Pause");
    cout << "stop threade  " << this_thread::get_id() << endl;
}

void foo2(thread& temp) {
    cout << "star threade  " << this_thread::get_id() << endl;
    thread t1(foo1); //Главное чтобы жыл объект\. вызвавший поток другой
    this_thread::sleep_for(std::chrono::milliseconds(500));
    temp = move(t1);
    cout << "stop threade  " << this_thread::get_id() << endl;

}


int main1()

{
    //
    //thread t1(foo2);
    //t1.join();
    //-----------------------

    thread t0; //чтобы при удалении потока другой жил
    thread t2(foo2, ref(t0));
    t2.join();
    if (t0.joinable()) {
        t0.join();
    }

    return 0;
}
