#include <iostream>
#include <Windows.h>

// Функция для вывода динамического массива с учетом логического и фактического размеров
void print_dynamic_array(int* arr, int logical_size, int actual_size) {
    for (int i = 0; i < actual_size; ++i) {
        if (i < logical_size) {
            std::cout << arr[i] << " "; // Выводим значение элемента
        }
        else {
            std::cout << "_ "; // Если элемент "в запасе" — выводим '_'
        }
    }
    std::cout << std::endl;
}

// Функция добавления элемента в динамический массив с возможным увеличением размера массива
// arr - ссылка на указатель на массив, logical_size - ссылка на логический размер,
// actual_size - ссылка на фактический размер, value - добавляемое значение
void append_to_dynamic_array(int*& arr, int& logical_size, int& actual_size, int value) {
    // Если есть свободное место (логический размер меньше фактического)
    if (logical_size < actual_size) {
        arr[logical_size] = value;
        ++logical_size;
    }
    // Если свободного места нет, увеличиваем массив в 2 раза
    else {
        int new_actual_size = actual_size * 2;
        // Если массив нулевого размера, делаем его размером 1
        if (new_actual_size == 0) new_actual_size = 1;
        int* new_arr = new int[new_actual_size]; //Указатель на новый массив

        // Копируем старые элементы
        for (int i = 0; i < logical_size; ++i) {
            new_arr[i] = arr[i];
        }
        // Добавляем новый элемент в конец
        new_arr[logical_size] = value;
        ++logical_size;
        // Старый массив больше не нужен, освобождаем память
        delete[] arr; //Освобождаем память, врезультате указатель пока пустой
        arr = new_arr; // Переназначаем указатель 
        actual_size = new_actual_size;
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
     
    int actual_size;
    int logical_size;

    // Ввод фактического и логического размеров
    std::cout << "Введите фактичеcкий размер массива: ";
    std::cin >> actual_size;

    std::cout << "Введите логический размер массива: ";
    std::cin >> logical_size;

    // Проверка корректности размеров
    if (logical_size > actual_size || actual_size < 0 || logical_size < 0) {
        std::cout << "Ошибка: некорректные размеры массива." << std::endl;
        return 1;
    }

    // Выделяем память под массив
    int* arr = new int[actual_size];

    // Вводим значения массива
    for (int i = 0; i < logical_size; ++i) {
        std::cout << "Введите arr[" << i << "]: ";
        std::cin >> arr[i];
    }

    std::cout << "Динамический массив: ";
    print_dynamic_array(arr, logical_size, actual_size);

    // Постоянно принимаем новые значения, пока пользователь не введет 0
    while (true) {
        std::cout << "Введите элемент для добавления: ";
        int value;
        std::cin >> value;
        if (value == 0) break; // Пользователь завершил ввод

        append_to_dynamic_array(arr, logical_size, actual_size, value);
        std::cout << "Динамический массив: ";
        print_dynamic_array(arr, logical_size, actual_size);
    }

    std::cout << "Спасибо! Ваш массив: ";
    print_dynamic_array(arr, logical_size, actual_size);

    // Освобождаем память
    delete[] arr;

    return 0;
}
