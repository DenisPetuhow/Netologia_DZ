
#include <iostream>
#include <windows.h>

int static function(const int m_arr[], int m_size, int m_number) {
    int left { 0 };
    int right{ m_size };

    while(left < right) {
        int mid = left + (right - left) / 2;
        if (m_arr[mid] <= m_number) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }

    return m_size - left;
    

}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int arr[] = { 14, 16, 19, 32, 32, 32, 56, 69, 72 };
    const int size = sizeof(arr) / sizeof(arr[0]);
    int pivot;

    std::cout << "Введите точку отсчёта: ";
    std::cin >> pivot;

    int count = function(arr, size, pivot);
    std::cout << " количество элементов больше введенного числа: " << pivot << ':' << count << std::endl;

    return 0;
}

