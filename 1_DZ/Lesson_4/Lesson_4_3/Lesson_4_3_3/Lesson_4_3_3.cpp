#include <iostream>
#include <Windows.h>
#include <cstring> // Для memset

void count_sort(int* arr, int size) {
    const int min_val = 10;
    const int max_val = 24;
    const int range = max_val - min_val + 1;

    int count[range]; // Создаём массив для подсчёта
    std::memset(count, 0, sizeof(count)); // Обнуляем массив

    // 1. Подсчитываем количество каждого элемента
    for (int i = 0; i < size; ++i) {
        count[arr[i] - min_val]++;
    }

    // 2. Восстанавливаем отсортированный массив
    int index = 0;
    for (int num = min_val; num <= max_val; ++num) {
        while (count[num - min_val] > 0) {
            arr[index++] = num;
            count[num - min_val]--;
        }
    }
}

// Функция для тестирования
void test_count_sort(int* arr, int size, const char* test_name) {
    std::cout << "Тест: " << test_name << "\nИсходный массив: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }

    count_sort(arr, size);

    std::cout << "\nОтсортированный массив: ";
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Тестовые данные
    int test1[] = { 19,14,22,22,17,22,13,21,20,24,18,10,17,16,17,20,22,11,20,16,14,13,10,22,18,14,16,24,19,17 };
    int test2[] = { 16,17,14,20,22,20,17,22,16,19,23,24,20,22,21,18,14,16,17,21,10,11,19,23,11,11,17,17,11,21,17,11,17,16,12,11,16,22,23,16 };
    int test3[] = { 21,15,19,18,23,12,18,18,19,23,12,20,15,22,21,18,19,20,12,16,20,14,17,13,10,23,19,14,10,22,19,12,24,23,22,15,13,22,18,18,11,23,24,17,10 };

    // Запуск тестов
    test_count_sort(test1, sizeof(test1) / sizeof(test1[0]), "Тест 1");
    test_count_sort(test2, sizeof(test2) / sizeof(test2[0]), "Тест 2");
    test_count_sort(test3, sizeof(test3) / sizeof(test3[0]), "Тест 3");

    return 0;
}
