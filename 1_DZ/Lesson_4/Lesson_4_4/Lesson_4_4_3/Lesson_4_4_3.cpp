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


void remove_dynamic_array_head(int*& arr, int& logical_size, int& actual_size) {
    // Если есть свободное место (логический размер меньше фактического)
    if (logical_size == 0) {
        // Массив пуст - удалять нечего
        return;
    }
    int new_logical_size = logical_size - 1;
    // Сравнение «строго больше одной трети»
    if (new_logical_size > actual_size / 3) {
        // Просто сдвигаем элементы влево
        for (int i = 1; i < logical_size; ++i) {
            arr[i - 1] = arr[i];
        }
        --logical_size;
    }
    else {
        // Новый фактический размер — в 3 раза меньше, но хотя бы 1
        int new_actual_size = actual_size / 3;
        if (new_actual_size < 1) {
            new_actual_size = 1;
        }
        int* new_arr = new int[new_actual_size];
        // Копируем сдвинутые элементы
        for (int i = 1; i < logical_size; ++i) {
            if (i - 1 < new_actual_size)
                new_arr[i - 1] = arr[i];
        }
        --logical_size;
        actual_size = new_actual_size;
        delete[] arr;
        arr = new_arr;
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

    // Удаление по команде пользователя
    while (true) {
        std::string reply;
        std::cout << "Удалить первый элемент? ";
        std::cin >> reply;

        if (reply == "да" || reply == "Да" || reply == "ДА" || reply == "y" || reply == "Y") {
            if (logical_size == 0) {
                std::cout << "Невозможно удалить первый элемент, так как массив пустой. До свидания!\n";
                break;
            }
            remove_dynamic_array_head(arr, logical_size, actual_size);
            std::cout << "Динамический массив: ";
            print_dynamic_array(arr, logical_size, actual_size);
        }
        else {
            std::cout << "Спасибо! Ваш массив: ";
            print_dynamic_array(arr, logical_size, actual_size);
            break;
        }
    }
    // Освобождаем память
    delete[] arr;

    return 0;
}
