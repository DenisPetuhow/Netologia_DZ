#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>


using namespace std;

template<typename T>
class safe_queue
{
private:
    queue<T> q; // Внутренняя очередь для хранения задач
    mutex m; // Мьютекс для защиты доступа к очереди
    condition_variable cv;  // Условная переменная для уведомления потоков о новых задачах
    bool stop_flag; // Флаг для завершения работы

public:
    safe_queue();
    void push(T value); // МЕТОД PUSH - добавление задачи в очередь
    bool pop(T& result); //извлечение задачи из очереди
    bool empty(); // проверка, пуста ли очередь
    void stop(); //сигнал о завершении работы
    size_t size();

    ~safe_queue();
};

template<typename T>
safe_queue<T>::safe_queue() : stop_flag(false)
{

}

template<typename T>
safe_queue<T>::~safe_queue()
{
    stop();
}

template<typename T>// Защита мьютексом обеспечивает, что только один поток может добавлять задачу
void safe_queue<T>::push(T value)
{
    {
        lock_guard<mutex> lock_l(m);
        
        q.push(std::move(value)); // Добавляем элемент в очередь (безопасно, т.к. мьютекс заблокирован)
        cout << "  [QUEUE] Добавлена задача. Размер очереди: " << q.size() << endl;
    }//  Мьютекс освобождается ПЕРЕД notify_one()!
   
    cv.notify_one(); // Уведомляем ОДИН ожидающий поток, что появилась новая задача
}

template<typename T> // Защита мьютексом + условная переменная для ожидания задач
bool safe_queue<T>::pop(T& result)
{
    unique_lock<mutex> lock_u(m);
    cv.wait(lock_u, [this]() { return !q.empty() || stop_flag; }); // предикат проверяет условие. и если оно фолс, то снова засыпает и освобождает мьютекс
    if (stop_flag && q.empty()) { return  false; }
    //  ДОПОЛНИТЕЛЬНАЯ ПРОВЕРКА (на случай ложного пробуждения)
    if (q.empty()) {
        return false;
    }
    result = q.front();
    q.pop();
    cout << "  [QUEUE] Извлечена задача. Осталось: " << q.size() << endl;
    return true; //иначе Потоки думают, что пришел сигнал завершения и завершаются после первой задачи
}

template<typename T> //
bool safe_queue<T>::empty()
{
    lock_guard<mutex> lock_e(m); //защита чтения
    return  q.empty();
}

template<typename T>
size_t safe_queue<T>::size() {
    std::lock_guard<std::mutex> lock(m);
    return q.size();
}

template<typename T> //
void safe_queue<T>::stop()
{
    lock_guard<mutex> lock_S(m); //защита изменения флага
    stop_flag = true;
    cv.notify_all(); // Будим ВСЕ ожидающие потоки
}




