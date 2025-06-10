#include <iostream>
#include <Windows.h>
#include <iomanip>
using namespace std;

int global_step = 1; // Глобальная нумерация шагов

// Функция вывода фрагмента массива
void print_subarray(int* arr, int left, int right) {
    cout << "[";
    for (int i = left; i <= right; ++i) {
        cout << arr[i];
        if (i < right) cout << ", ";
    }
    cout << "]";
}

// Слияние с подробным отслеживанием уровня рекурсии, mid и шагов
void merge(int* arr, int left, int mid, int right, int level) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* A = new int[n1];
    int* B = new int[n2];
    for (int i = 0; i < n1; ++i) A[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) B[i] = arr[mid + 1 + i];

    cout << setw(abs(level) * 2) << ""
        << "ШАГ " << global_step++ << ": "
        << "Слияние на уровне " << level
        << " интервал [" << left << "," << right << "], mid=" << mid << ". "
        << "Левая часть: ";
    print_subarray(arr, left, mid);
    cout << "; Правая часть: ";
    print_subarray(arr, mid + 1, right);
    cout << endl;

    int ia = 0, ib = 0, ic = left;
    while (ia < n1 || ib < n2) {
        if (ia == n1) {
            arr[ic++] = B[ib++];
        }
        else if (ib == n2) {
            arr[ic++] = A[ia++];
        }
        else if (A[ia] <= B[ib]) {
            arr[ic++] = A[ia++];
        }
        else {
            arr[ic++] = B[ib++];
        }
    }
    cout << setw(abs(level) * 2) << ""
        << "      Результат слияния: ";
    print_subarray(arr, left, right);
    cout << endl;
    delete[] A;
    delete[] B;
}

// Рекурсивная сортировка с отслеживанием уровня, mid и шага
void merge_sort_recursive(int* arr, int left, int right, int level) {
    int mid = left + (right - left) / 2;
    cout << setw(abs(level) * 2) << ""
        << "ШАГ " << global_step++ << ": "
        << "Спуск на уровень " << level
        << ", интервал [" << left << "," << right << "]";
    if (left < right) {
        cout << ", mid=" << mid;
    }
    cout << ", массив ";
    print_subarray(arr, left, right);
    cout << endl;

    if (left >= right) return;
    merge_sort_recursive(arr, left, mid, level - 1);
    merge_sort_recursive(arr, mid + 1, right, level - 1);
    merge(arr, left, mid, right, level);
}

// Обёртка
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
    int arr[] = { 38, 27, 43, 3, 9, 82, 10};
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

    global_step = 0;
    int arr1[] = { 88, 91, 87, 59, 53, 49, 29, 16, 4, 27, 28, 89, 2, 25, 74 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    cout << "Исходный массив: ";
    print_array(arr1, size1);
    merge_sort(arr1, size1);
    cout << "Отсортированный массив: ";
    print_array(arr1, size1);

    return 0;
}