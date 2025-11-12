#pragma once
#include "save_queue.h"
#include <thread>
#include <vector>
#include <functional>

class thread_pool
{

private:
    vector<thread> workers;                           // Вектор рабочих потоков
    safe_queue<function<void()>> tasks;              // Потокобезопасная очередь задач (ЗДЕСЬ ПРОИСХОДИТ ЗАЩИТА МЬЮТЕКСАМИ!)
    void work(); // абочая функция каждого потока

public:
    thread_pool(size_t num_threads); // КОНСТРУКТОР - создаёт пул потоков
    // ДЕСТРУКТОР - корректно завершает работу пула
    ~thread_pool();
    void submit(function<void()> task); // Добавляем задачу в потокобезопасную очередь
    size_t size() const;

};
