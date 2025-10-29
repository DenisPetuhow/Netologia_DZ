#include <iostream>
#include <vector>
#include <future>
#include <algorithm>

using namespace std;

// Функция для асинхронного поиска минимального элемента в диапазоне
void findMinInRange(const vector<int>& arr, int start, promise<int> prom) {
    int minIndex = start;

    // Ищем минимальный элемент от start до конца массива
    for (int i = start + 1; i < arr.size(); i++) {
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
    }

    // Передаем результат через promise
    prom.set_value(minIndex);
}

int main() {

    setlocale(LC_ALL, "Russian");
    vector<int> numbers = { 64, 25, 12, 22, 11, 5, 90 };

    cout << "Исходный массив: ";
    for (int n : numbers) cout << n << " ";
    cout << endl << endl;

    // Сортировка выбором с асинхронным поиском
    for (int i = 0; i < numbers.size() - 1; i++) {
        cout << "Шаг " << i + 1 << ": ищем минимум с позиции " << i + 1 << endl;

        // Создаем promise и future
        promise<int> prom;
        future<int> fut = prom.get_future();

        // Запускаем асинхронный поиск минимального элемента
        async( findMinInRange, ref(numbers), i, move(prom));

        // Ждем результат от асинхронной задачи
        fut.wait();
        int minIndex = fut.get();

        cout << "Найден минимальный элемент " << numbers[minIndex]
            << " на позиции " << minIndex + 1 << endl;

        // Меняем местами текущий элемент с минимальным
        if (minIndex != i) {
            swap(numbers[i], numbers[minIndex]);
            cout << "Обмен: " << numbers[minIndex] << " <-> " << numbers[i] << endl;
        }

        // Показываем текущее состояние массива
        cout << "Текущий массив: ";
        for (int n : numbers) cout << n << " ";
        cout << endl << endl;
    }

    cout << "Отсортированный массив: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;

    return 0;
}