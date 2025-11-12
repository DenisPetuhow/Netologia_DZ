
#include <chrono>
#include "thread_pool.h"
using namespace std;

void function1() {
    cout << "  [F1] Выполняется function1 в потоке: " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(500));  // Имитация работы
    cout << "  [F1] Завершена" << endl;
}

void function2() {
    cout << "  [F2] Выполняется function2 в потоке: " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(500));  // Имитация работы
    cout << "  [F2] Завершена" << endl;
}

void function3() {
    cout << "  [F3] Выполняется function3 в потоке: " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::milliseconds(500));  // Имитация работы
    cout << "  [F3] Завершена" << endl;
}

int main()
{
    setlocale(LC_ALL, "Rus");

    // Определяем количество потоков = количеству аппаратных ядер процессора
    size_t num_threads = thread::hardware_concurrency();
    cout << "==================================================" << endl;
    cout << "Создание пула из " << num_threads << " потоков" << endl;
    cout << "==================================================" << endl;

    // Создаём пул потоков
    thread_pool pool(num_threads);

    this_thread::sleep_for(chrono::milliseconds(500)); // Пауза для вывода

    cout << "\n==================================================" << endl;
    cout << "Начинаем отправку задач (по 2 функции раз в секунду)" << endl;
    cout << "==================================================" << endl;


    // Отправляем задачи в пул раз в секунду
    for (int i = 0; i < 5; i++) {
        cout << "\n>>> Итерация " << i + 1 << " <<<" << endl;

        // Добавляем 2 функции одновременно
        pool.submit(function1);
        pool.submit(function2);
        pool.submit(function3);

        // Ждём 1 секунду перед следующей итерацией (как требуется в задании)
        this_thread::sleep_for(chrono::seconds(2));
    }

 
    std::cout << "\nТЕСТ 3: Массовая отправка задач \n";
    for (int i = 0; i < 10; i++) {
        pool.submit([i]() {
            std::cout << "   Задача #" << i << " - Выполняется в потоке: "
                << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            });
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "____________________________________________\n";

    // Даём время на завершение последних задач
    cout << "\n==================================================" << endl;
    cout << "Ожидание завершения задач..." << endl;
    cout << "==================================================" << endl;
    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n==================================================" << endl;
    cout << "Завершение программы и уничтожение пула" << endl;
    cout << "==================================================" << endl;

    // Деструктор thread_pool автоматически вызовется здесь
    return 0;
}
