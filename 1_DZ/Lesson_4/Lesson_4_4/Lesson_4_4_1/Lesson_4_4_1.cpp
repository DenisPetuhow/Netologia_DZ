
#include <iostream>
#include <Windows.h>

void print_dynamic_array(int* arr, int logical_size, int actual_size) {
    for (int i = 0; i < actual_size; ++i) {
        if (i < logical_size) {
            std::cout << arr[i] << " "; // Выводим элемент, если он входит в логический размер
        }
        else {
            std::cout << "_ ";  // Выводим символ "_" для неиспользованных мест
        }
    }
    std::cout << std::endl;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int actual_size{ };   // Максимальный размер массива
    int logical_size{ };  // Количество реально добавленных элементов

    std::cout << "Введите фактичеcкий размер массива: ";
    std::cin >> actual_size;
    std::cout << "Введите логическийcкий размер массива: ";
    std::cin >> logical_size;

    if (actual_size < logical_size) {
        std::cout << "Ошибка!Логический размер массива не может превышать фактический!" << std::endl;
        return 1; // Завершаем программу с ошибкой
    }

    // Выделяем память под массив фактического размера
    int* arr = new int[actual_size];

    std::cout << "Введите елементы массива: " << std::endl;
    for (int i = 0; i < logical_size ; ++i) {
        std::cout << "Введите елемент массива arr[" << i << "]: ";
        std::cin >> arr[i];
    }
    std::cout << "Динамический массив: ";
    print_dynamic_array(arr, logical_size, actual_size);


    // Освобождаем память
    delete[] arr;
    return 0;
}
