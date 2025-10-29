#include <algorithm>
#include <chrono>
#include <execution>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <numeric>
#include "Timer.h"

using namespace std;

namespace
{
    // Рекурсивная реализация параллельного for_each
    template<typename Iter, typename Func>
    void parallel_for_each(Iter first, Iter last, Func func) {
        // Вычисляем размер контейнера
        size_t size = distance(first, last);

        // Если элементов мало (меньше 10), обрабатываем в одном потоке
        // Это предотвращает избыточное создание потоков для маленьких задач
        if (size < 10) {
            // Используем стандартный for_each для последовательной обработки
            std::for_each(first, last, func);
        }
        else {
            // Находим середину контейнера
            Iter middle = first;
            advance(middle, size / 2);

            // Запускаем асинхронную задачу для левой половины
            // async запускает функцию в отдельном потоке (если возможно)
            auto future_left = async(parallel_for_each<Iter, Func>, first, middle, func); // Возвращает std::future, который позволяет отследить завершение задачи.
            // Рекурсивно обрабатываем правую половину в текущем потоке
            parallel_for_each(middle, last, func);

            // Ждем завершения левой половины
            // get() блокирует выполнение
            future_left.get();// Блокирует выполнение текущего потока, пока асинхронная задача не завершится.
        }
    }
}

// Пример функции для обработки элементов
void print_and_modify(int& x) {
    cout << x << " ";
}

int main()
{

    setlocale(LC_ALL, "Russian");
    // Создаем вектор из 20 случайных чисел
    vector<int> V1(20);

    // Заполняем вектор случайными числами от 1 до 10
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);

    for (auto& elem : V1) {
        elem = dis(gen);
    }

    cout << "Исходные элементы: ";
    for (const auto& elem : V1) {
        cout << elem << " ";
    }
    cout << endl;

    // Запускаем параллельный for_each с функцией обработки
    Timer T("par_for_each");
    parallel_for_each(V1.begin(), V1.end(), print_and_modify);
    T.print();

    T.start("for_each");
    for_each(V1.begin(), V1.end(), print_and_modify);
    T.print();

    return 0;
}