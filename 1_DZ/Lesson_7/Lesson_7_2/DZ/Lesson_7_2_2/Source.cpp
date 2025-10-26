#include <iostream>
#include <thread>          
#include <vector>          // Для хранения потоков
#include <mutex>           // Для синхронизации доступа к консоли
#include <chrono>          // Для работы со временем
#include <iomanip>         // Для форматирования вывода
#include <random>          // Для генерации случайных задержек

using namespace std;


mutex console_mutex;


void moveCursor(int row, int col) { // ФУНКЦИЯ ДЛЯ ПЕРЕМЕЩЕНИЯ КУРСОРА В КОНСОЛИ // Используется для обновления прогресс-баров без "прокрутки" консоли
    // ANSI escape-последовательность для перемещения курсора
    // \033[ - начало escape-последовательности
    // row;col - координаты (строка;колонка)
    // H - команда перемещения курсора
    cout << "\033[" << row << ";" << col << "H";
}

void clearScreen() { // ФУНКЦИЯ ДЛЯ ОЧИСТКИ ЭКРАНА     // ANSI escape-последовательность для очистки экрана

    cout << "\033[2J\033[1;1H";
}

void threadFunction(int threadNumber, int progressBarLength, int totalThreads) { // ФУНКЦИЯ ПОТОКА (выполняется в отдельном потоке)
    /*
     * threadNumber - номер потока по порядку (0, 1, 2...)
     * progressBarLength - длина прогресс-бара (количество символов)
     * totalThreads - общее количество потоков
     */

     // Получаем ID текущего потока (уникальный идентификатор)
    thread::id threadId = this_thread::get_id();

    // Засекаем время начала работы потока
    auto startTime = chrono::high_resolution_clock::now();

    // Генератор случайных чисел для имитации разной скорости работы
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(50, 150); // Задержка от 50 до 150 мс

    // ============================================
    // ОСНОВНОЙ ЦИКЛ ЗАПОЛНЕНИЯ ПРОГРЕСС-БАРА
    // ============================================

    for (int i = 0; i <= progressBarLength; i++) { //для каждого раза запучкается заново и перерисовывается
        // --- КРИТИЧЕСКАЯ СЕКЦИЯ (защищена mutex) ---
        // Только один поток может выполнять код внутри фигурных скобок
        {
            // lock_guard автоматически блокирует mutex при создании
            // и разблокирует при выходе из области видимости {}
            lock_guard<mutex> lock(console_mutex);

            // Вычисляем номер строки для текущего потока
            // Строка 0 - заголовок, поток 0 на строке 1, поток 1 на строке 2 и т.д.
            int row = threadNumber + 2;

            // Перемещаем курсор в начало строки потока
            moveCursor(row, 1);

            // Выводим номер потока (выровнено по правому краю, ширина 2 символа)
            cout << setw(2) << threadNumber << " | ";

            // Выводим ID потока (ширина 8 символов)
            cout << setw(8) << threadId << " | ";

            // --- РИСУЕМ ПРОГРЕСС-БАР ---
            cout << "[";

            // Заполненная часть (символ █)
            for (int j = 0; j < i; j++) {
                cout << "█";
            }

            // Незаполненная часть (пробелы)
            for (int j = i; j < progressBarLength; j++) {
                cout << " ";
            }

            cout << "] ";

            // --- ВЫЧИСЛЯЕМ И ВЫВОДИМ ПРОЦЕНТ ВЫПОЛНЕНИЯ ---
            int percentage = (i * 100) / progressBarLength;
            cout << setw(3) << percentage << "% ";

            // --- ЕСЛИ ЗАВЕРШЕНО - ВЫВОДИМ ВРЕМЯ ---
            if (i == progressBarLength) {
                // Вычисляем прошедшее время
                auto endTime = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

                // Выводим время в формате X.XXs
                cout << "| Time: " << fixed << setprecision(2)
                    << duration.count() / 1000.0 << "s";
            }

            // Сбрасываем буфер вывода (принудительная отправка в консоль)
            cout << flush;  // гарантирует, что символы сразу уходят в консоль, иначе бар «дергался» бы.
        }
        // --- КОНЕЦ КРИТИЧЕСКОЙ СЕКЦИИ ---
        // Здесь lock_guard автоматически разблокирует mutex

        // Имитация работы потока - случайная задержка
        // Это нужно, чтобы потоки работали с разной скоростью
        if (i < progressBarLength) {
            this_thread::sleep_for(chrono::milliseconds(dis(gen)));
        }
    }
}
// ============================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================

int main() {
    setlocale(LC_ALL, "Russian");

    const int THREAD_COUNT = 5;         // Количество потоков
    const int PROGRESS_BAR_LENGTH = 30; // Длина прогресс-бара в символах
    clearScreen();

    cout << " # | id       | Progress Bar                           | Time\n";
    cout << "---|----------|----------------------------------------|------------\n";

    // Выделяем место под прогресс-бары (пустые строки)
    for (int i = 0; i < THREAD_COUNT; i++) {
        cout << "\n";
    }
    vector<thread> threads;     // СОЗДАНИЕ И ЗАПУСК ПОТОКОВ

    // Создаём и запускаем потоки
    for (int i = 0; i < THREAD_COUNT; i++) {
        // emplace_back создаёт новый поток и добавляет его в вектор
        // Поток сразу начинает выполнять функцию threadFunction
        threads.emplace_back(threadFunction, i, PROGRESS_BAR_LENGTH, THREAD_COUNT); //emplace_back Внутри вектора конструируется объект прямо «на месте» – лишнее перемещение / копия не делается.

        /* Что происходит:
         * 1. Создаётся новый поток
         * 2. В потоке вызывается функция threadFunction с параметрами:
         *    - i (номер потока)
         *    - PROGRESS_BAR_LENGTH (длина прогресс-бара)
         *    - THREAD_COUNT (общее количество потоков)
         * 3. Поток начинает работать параллельно с главным потоком
         */
    }

    // Главный поток будет ждать здесь, пока все рабочие потоки не закончат работу
    for (auto& t : threads) {     // ОЖИДАНИЕ ЗАВЕРШЕНИЯ ВСЕХ ПОТОКОВ
        t.join(); // Ждём завершения каждого потока
    }
    // Перемещаем курсор после всех прогресс-баров
    moveCursor(THREAD_COUNT + 3, 1);
    cout << "\nAll threads completed!\n";

    return 0;
}