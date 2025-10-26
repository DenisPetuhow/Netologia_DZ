#include <iostream>
#include <thread>         // для работы с потоками
#include <chrono>         // для работы со временем (задержки)

using namespace std;

// ============================================================================
// ОСНОВНОЕ ИЗМЕНЕНИЕ: Используем atomic вместо обычного int
// ============================================================================
// atomic<int> гарантирует, что операции с переменной будут атомарными
// (неделимыми) - т.е. один поток не может "прервать" другой поток
// в середине операции чтения/записи

atomic<int> clientCounter (0);


int maxClients = 10;


void clientThread() {
    // Счётчик пришедших клиентов
    int arrivedClients = 0;

    while (arrivedClients < maxClients) {
        this_thread::sleep_for(chrono::seconds(1));

        // Оператор ++ работает атомарно для atomic<int>
     // По умолчанию использует memory_order_seq_cst (самый строгий порядок)
     // Это гарантирует, что:
     // 1. Операция выполнится полностью, без прерывания
     // 2. Все потоки увидят изменения в одинаковом порядке
        clientCounter++;
        arrivedClients++;

        cout << "Пришёл клиент! Клиентов в очереди: " << clientCounter
            << " (всего пришло: " << arrivedClients << ")" << endl;
    }

    cout << "\n>>> Все клиенты пришли! Больше клиентов не будет.\n" << endl;
}


void operatorThread() {
    // Счётчик обслуженных клиентов
    int servedClients = 0;


    while (servedClients < maxClients) {
        this_thread::sleep_for(chrono::seconds(2));


        if (clientCounter.load() > 0) {        // load() - атомарное чтение значения// По умолчанию использует memory_order_seq_cst

            clientCounter--; // Оператор -- работает атомарно
            servedClients++;


            cout << "Операционист обслужил клиента! Осталось в очереди: "
                << clientCounter << " (обслужено: " << servedClients << ")" << endl;
        }
        else {

            cout << "Операционист ждёт... Очередь пуста." << endl;
        }
    }

    cout << "\n>>> Операционист обслужил всех клиентов! Работа завершена.\n" << endl;
}


int main() {

    setlocale(LC_ALL, "Russian");

    cout << "=== Симуляция очереди клиентов ===" << endl;
    cout << "Максимальное количество клиентов: " << maxClients << endl;
    cout << "Клиенты приходят каждую 1 секунду" << endl;
    cout << "Операционист обслуживает каждые 2 секунды" << endl;
    cout << "====================================\n" << endl;


    thread client(clientThread);
    thread oper(operatorThread);

    client.join();
    oper.join();

    cout << "=== Программа завершена ===" << endl;
    cout << "Итоговое значение счётчика: " << clientCounter << endl;

    return 0;
}