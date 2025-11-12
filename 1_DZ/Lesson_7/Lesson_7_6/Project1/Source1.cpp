#include <iostream>
#include <semaphore>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <barrier>

using namespace std;



int main2()
{
    /*jthread t1([]() {
        int cnt = 0;
        while (cnt < 10) {
            this_thread::sleep_for(200ms);
            cout << cnt++ << endl;
        }
        });
*/
    jthread t2([](stop_token stop_flag) { //stop_flag спец флаг(атомарный, который срабатывает при запуске request_stop()
        stop_callback(stop_flag, []() { cout << "stopppp" << endl; }); // активируется если случается остановка
        int cnt = 0;
        while (cnt < 10) {
            if (stop_flag.stop_requested()) { //если есть флагто срабатывает  request_stop()
                break;
            }
            this_thread::sleep_for(200ms);
            cout << cnt++ << endl;
        }
        });
    this_thread::sleep_for(0.5s);
    //t2.request_stop(); // передает через флаг остановку
    system("Pause");
    return 0;
}
