#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>      // для форматирования вывода таблицы
#include <string>

using namespace std;

/**
 * Функция для суммирования части двух векторов
 * Каждый поток будет выполнять эту функцию для своей части массива
 *
 * @param vec1 - первый вектор (входной)
 * @param vec2 - второй вектор (входной)
 * @param result - вектор результата (выходной)
 * @param start - начальный индекс для обработки
 * @param end - конечный индекс для обработки
 */
void sumVectorsPart(const vector<int>& vec1,
    const vector<int>& vec2,
    vector<int>& result,
    int start,
    int end) {
    // Проходим по элементам от start до end
    for (int i = start; i < end; i++) {
        // Складываем соответствующие элементы
        result[i] = vec1[i] + vec2[i];
    }
}

/**
 * Функция для параллельного вычисления суммы векторов
 *
 * @param vec1 - первый вектор
 * @param vec2 - второй вектор
 * @param result - вектор для результата
 * @param numThreads - количество потоков для вычисления
 * @return время выполнения в миллисекундах
 */
double parallelVectorSum(const vector<int>& vec1,
    const vector<int>& vec2,
    vector<int>& result,
    int numThreads) {

    int size = vec1.size();  // размер векторов

    // Создаём массив для хранения потоков
    vector<thread> threads;

    // Вычисляем размер части массива для каждого потока
    int chunkSize = size / numThreads;

    // Засекаем время начала выполнения
    auto startTime = chrono::high_resolution_clock::now();

    // Создаём и запускаем потоки
    for (int i = 0; i < numThreads; i++) {
        // Вычисляем начальный индекс для текущего потока
        int start = i * chunkSize;

        // Вычисляем конечный индекс
        // Для последнего потока берём все оставшиеся элементы
        int end;
        if (i == numThreads - 1) {
            end = size;  // последний поток обрабатывает остаток
        }
        else {
            end = start + chunkSize;
        }

        // Создаём поток и добавляем его в массив
        // ref() используется для передачи параметров по ссылке
        threads.push_back(thread(sumVectorsPart,
            ref(vec1),
            ref(vec2),
            ref(result),
            start,
            end));
    }

    // Ждём завершения всех потоков
    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    // Засекаем время окончания
    auto endTime = chrono::high_resolution_clock::now();

    // Вычисляем время выполнения в миллисекундах
    chrono::duration<double, milli> duration = endTime - startTime;

    return duration.count();  // возвращаем время в мс
}

/**
 * Функция для инициализации вектора случайными значениями
 */
void initVector(vector<int>& vec, int size) {
    vec.resize(size);
    for (int i = 0; i < size; i++) {
        vec[i] = rand() % 100;  // случайные числа от 0 до 99
    }
}

/**
 * Функция для вывода красивой таблицы результатов
 */
void printTable(const vector<vector<double>>& results,
    const vector<int>& sizes,
    const vector<int>& threadCounts) {

    cout << "\n=== ТАБЛИЦА РЕЗУЛЬТАТОВ (время в миллисекундах) ===" << endl;
    cout << string(70, '=') << endl;

    // Заголовок таблицы
    cout << setw(15) << "Размер" << " | ";
    for (int threads : threadCounts) {
        cout << setw(10) << (to_string(threads) + " пот.") << " | ";
    }
    cout << endl;
    cout << string(70, '-') << endl;

    // Строки с данными
    for (int i = 0; i < sizes.size(); i++) {
        cout << setw(15) << sizes[i] << " | ";
        for (int j = 0; j < threadCounts.size(); j++) {
            cout << setw(10) << fixed << setprecision(3) << results[i][j] << " | ";
        }
        cout << endl;
    }
    cout << string(70, '=') << endl;
}

/**
 * Функция для поиска и вывода оптимального количества потоков
 */
void analyzeResults(const vector<vector<double>>& results,
    const vector<int>& sizes,
    const vector<int>& threadCounts) {

    cout << "\n=== АНАЛИЗ РЕЗУЛЬТАТОВ ===" << endl;
    cout << string(70, '=') << endl;

    for (int i = 0; i < sizes.size(); i++) {
        // Находим минимальное время и соответствующее количество потоков
        double minTime = results[i][0];
        int bestThreads = threadCounts[0];

        for (int j = 1; j < threadCounts.size(); j++) {
            if (results[i][j] < minTime) {
                minTime = results[i][j];
                bestThreads = threadCounts[j];
            }
        }

        cout << "Размер " << setw(10) << sizes[i] << ": "
            << "Оптимально " << setw(2) << bestThreads << " потоков "
            << "(время: " << fixed << setprecision(3) << minTime << " мс)"
            << endl;
    }
    cout << string(70, '=') << endl;
}

/**
 * Главная функция программы
 */
int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== ПАРАЛЛЕЛЬНОЕ ВЫЧИСЛЕНИЕ СУММЫ ВЕКТОРОВ ===" << endl;
    cout << string(70, '=') << endl;

    // Выводим количество доступных аппаратных ядер
    unsigned int hardwareCores = thread::hardware_concurrency();
    cout << "\nДоступное количество аппаратных ядер: " << hardwareCores << endl;
    cout << string(70, '=') << endl;

    // Массивы с параметрами тестирования
    vector<int> sizes = { 1000, 10000, 100000, 1000000 };           // размеры векторов
    vector<int> threadCounts = { 2, 4, 8, 16 };                     // количество потоков

    // Двумерный массив для хранения результатов
    // results[i][j] = время для sizes[i] и threadCounts[j]
    vector<vector<double>> results(sizes.size(), vector<double>(threadCounts.size()));

    // Основной цикл тестирования
    cout << "\nНачинаем тестирование...\n" << endl;

    for (int i = 0; i < sizes.size(); i++) {
        int size = sizes[i];

        cout << "Тестирование для размера: " << size << " элементов" << endl;

        // Создаём и инициализируем векторы
        vector<int> vec1, vec2, result;
        initVector(vec1, size);
        initVector(vec2, size);
        result.resize(size);

        // Тестируем разное количество потоков
        for (int j = 0; j < threadCounts.size(); j++) {
            int threads = threadCounts[j];

            cout << "  Потоков: " << setw(2) << threads << " -> ";

            // Выполняем вычисление и замеряем время
            double time = parallelVectorSum(vec1, vec2, result, threads);
            results[i][j] = time;

            cout << fixed << setprecision(3) << time << " мс" << endl;
        }
        cout << endl;
    }

    // Выводим таблицу результатов
    printTable(results, sizes, threadCounts);

    // Анализируем результаты
    analyzeResults(results, sizes, threadCounts);

    // Дополнительная проверка корректности вычислений
    cout << "\n=== ПРОВЕРКА КОРРЕКТНОСТИ ===" << endl;
    vector<int> testVec1 = { 1, 2, 3, 4, 5 };
    vector<int> testVec2 = { 10, 20, 30, 40, 50 };
    vector<int> testResult(5);

    parallelVectorSum(testVec1, testVec2, testResult, 2);

    cout << "Вектор 1: ";
    for (int val : testVec1) cout << val << " ";
    cout << "\nВектор 2: ";
    for (int val : testVec2) cout << val << " ";
    cout << "\nРезультат: ";
    for (int val : testResult) cout << val << " ";
    cout << "\n(Ожидалось: 11 22 33 44 55)" << endl;

    cout << "\n=== ПРОГРАММА ЗАВЕРШЕНА ===" << endl;

    return 0;
}