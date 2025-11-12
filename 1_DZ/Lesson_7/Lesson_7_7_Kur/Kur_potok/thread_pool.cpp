#include "thread_pool.h"


thread_pool::thread_pool(size_t num_threads)// num_threads - количество рабочих потоков в пуле
{
    // Создаём указанное количество рабочих потоков
    for (size_t i = 0; i < num_threads; i++) {
        workers.emplace_back(thread(&thread_pool::work, this));  // Каждый поток будет выполнять метод work() текущего объекта
    }
    cout << " Количество созданных потоков " << num_threads << endl;
}

thread_pool::~thread_pool()// num_threads - количество рабочих потоков в пуле
{
    cout << " остановка потоков " << endl;
    tasks.stop();   // сигнал остановки очереди задач
    // Ждём завершения всех рабочих потоков
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();       // join() блокирует выполнение до завершения потока
        }
    }
    cout << " ВСЕ потоки остановлены " << endl;
}

void thread_pool::work()
{
    // Выводим ID потока при его создании
    cout << "Поток запущен, ID: " << this_thread::get_id() << endl;
    while (true) {
        function<void()> task;     // Переменная для хранения задачи

        // Пытаемся получить задачу из потокобезопасной очереди
        // Метод pop() блакирует поток если очередь пуста
        if (tasks.pop(task)) {
            cout << "Поток " << this_thread::get_id() << " начал выполнение задачи" << endl;
            try {
                task();                 // Выполняем полученную задачу
            }
            catch (const std::exception& e) {
                cerr << " исключение в задаче " << e.what() << endl;
            }

        }
        else {
            // Если pop вернул false - это сигнал завершения
            break;
        }
    }
    cout << "Поток завершен, ID: " << this_thread::get_id() << endl;
}

// task - функция без параметров и возвращаемого значения
void thread_pool::submit(function<void()> task) { // Добавляем задачу в потокобезопасную очередь

    tasks.push(task); // Метод push() ЗАЩИЩЁН МЬЮТЕКСОМ внутри safe_queue, push сам уведомит поток о появлении задачи
}

size_t thread_pool::size() const {
    return workers.size();
}
