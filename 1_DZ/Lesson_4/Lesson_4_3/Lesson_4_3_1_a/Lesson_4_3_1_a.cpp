#include <iostream>
#include <Windows.h>
#include <iomanip>
using namespace std;

void print_subarray(int* arr, int left, int right) {
    cout << "[";
    for (int i = left; i <= right; ++i) {
        cout << arr[i];
        if (i < right) cout << ", ";
    }
    cout << "]";
}


// Вспомогательная функция для слияния двух отсортированных подмассивов
// arr указывает на начало массива
// left — начало первого подмассива, mid — конец первого (середина), right — конец второго
void merge(int* arr, int left, int mid, int right, int depth) {
    int n1 = mid - left + 1; // длина первого подмассива
    int n2 = right - mid;    // длина второго подмассива

    // Создаём временные массивы для копий подмассивов
    int* A = new int[n1];
    int* B = new int[n2];

    for (int i = 0; i < n1; ++i) A[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) B[i] = arr[mid + 1 + i];

    cout << setw(depth * 2) << "" << "Слияние ";
    print_subarray(arr, left, mid);
    cout << " и ";
    print_subarray(arr, mid + 1, right);
    cout << " => ";
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

    print_subarray(arr, left, right);
    cout << endl;
    delete[] A;
    delete[] B;
}

// Рекурсивная функция для деления массива и вызова слияния
void merge_sort_recursive(int* arr, int left, int right, int depth) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort_recursive(arr, left, mid, depth + 1);
    merge_sort_recursive(arr, mid + 1, right, depth + 1);
    merge(arr, left, mid, right, depth);
}
// Основная функция по заданной сигнатуре
void merge_sort(int* arr, int size) {
    merge_sort_recursive(arr, 0, size - 1, 0);
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