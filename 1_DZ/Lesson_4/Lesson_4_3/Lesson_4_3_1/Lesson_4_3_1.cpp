#include <iostream>
#include <Windows.h>
using namespace std;

int a = 0;
int b = 0;

// Вспомогательная функция для слияния двух отсортированных подмассивов
// arr указывает на начало массива
// left — начало первого подмассива, mid — конец первого (середина), right — конец второго
void merge(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1; // длина первого подмассива
    int n2 = right - mid;    // длина второго подмассива

    // Создаём временные массивы для копий подмассивов
    int* A = new int[n1];
    int* B = new int[n2];

    for (int i = 0; i < n1; ++i) A[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) B[i] = arr[mid + 1 + i];

    // Индексы для A, B и исходного массива arr
    int ia = 0, ib = 0, ic = left;

    // Основной цикл слияния по вашему псевдокоду:
    while (ia < n1 || ib < n2) {
        if (ia == n1) { // A закончился — берём из B
            arr[ic] = B[ib];
            ib++;
        }
        else if (ib == n2) { // B закончился — берём из A
            arr[ic] = A[ia];
            ia++;
        }
        else {
            if (A[ia] <= B[ib]) {
                arr[ic] = A[ia];
                ia++;
            }
            else {
                arr[ic] = B[ib];
                ib++;
            }
        }
        ic++;
    }
    delete[] A;
    delete[] B;
}

// Рекурсивная функция для деления массива и вызова слияния
void merge_sort_recursive(int* arr, int left, int right) {
    if (left >= right) return; // базовый случай — 0 или 1 элемент уже отсортированы

    int mid = left + (right - left) / 2; // Находим середину
    // Сортируем рекурсивно левую и правую части
    cout << "A: " << ++a <<  endl;
    merge_sort_recursive(arr, left, mid);
    cout << "B: " << ++b  << endl;
    merge_sort_recursive(arr, mid + 1, right);

    // Сливаем две отсортированные части
    merge(arr, left, mid, right);
}

// Основная функция по заданной сигнатуре
void merge_sort(int* arr, int size) {
    merge_sort_recursive(arr, 0, size - 1);
}

// Функция для печати массива
void print_array(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Тестирование
int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int arr[] = { 3, 43, 38, 29, 18, 72, 57, 61, 2, 33 };
    int size = sizeof(arr) / sizeof(arr[0]);
    cout << "Исходный массив: ";
    print_array(arr, size);

    // Подробная поэтапная сортировка:
    // 1. Массив делится пополам: [3,43,38,29,18] и [72,57,61,2,33]
    // 2. Рекурсивно каждая часть снова делится: [3,43,38], [29,18], [72,57], [61,2,33]
    // 3. Каждая часть сортируется рекурсивно до длины 1
    // 4. Слияние: пары сравниваются и сливаются в отсортированном порядке наверх рекурсии
    // 5. В итоге весь массив будет отсортирован

    merge_sort(arr, size);

    cout << "Отсортированный массив: ";
    print_array(arr, size);
    
    int arr1[] = { 88, 91, 87, 59, 53, 49, 29, 16, 4, 27, 28, 89, 2, 25, 74 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "Исходный массив: ";
    print_array(arr1, size1);
    merge_sort(arr1, size1);
    cout << "Отсортированный массив: ";
    print_array(arr1, size1);

    return 0;
}