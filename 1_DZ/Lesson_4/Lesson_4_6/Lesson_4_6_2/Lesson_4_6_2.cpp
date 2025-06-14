
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

// Перечисление команд
enum class Command {
    Вверх = 1,
    Влево,
    Вправо,
    Выход,
    Ошибка
};

// Функция для вывода элемента пирамиды
void print_element(const std::vector<int>& heap, int idx) {
    int level = 0, j = idx;
    while (j > 0) {
        j = (j - 1) / 2;
        ++level;
    }
    if (idx == 0) {
        std::cout << level << " root " << heap[idx] << std::endl;
    }
    else {
        int parent = (idx - 1) / 2;
        if (idx == 2 * parent + 1) {
            std::cout << level << " left(" << heap[parent] << ") " << heap[idx] << std::endl;
        }
        else {
            std::cout << level << " right(" << heap[parent] << ") " << heap[idx] << std::endl;
        }
    }
}

// Функция для вывода всей пирамиды
void print_pyramid(const std::vector<int>& heap) {
    for (int i = 0; i < static_cast<int>(heap.size()); ++i) {
        print_element(heap, i);
    }
}

// Функция для организации навигации пользователя по пирамиде
void travel_pyramid(const std::vector<int>& heap) {
    int idx = 0;
    int cmd_ind;
    std::cout << "У вас есть 4 команды: 1-Верх, 2-Влево, 3-Вправо, 4-Выход: Введите номер команды: " << std::endl;

    while (true) {
        std::cout << "Вы находитесь здесь: ";
        print_element(heap, idx);
        std::cout << "Введите команду: ";
        std::cin >> cmd_ind;
        Command numberCommand = static_cast<Command>(cmd_ind); //Переводим чтобы экземпляр принял номер индекса


        switch (numberCommand) {
        case Command::Вверх:
            if (idx == 0) {
                std::cout << "Ошибка! Отсутствует родитель" << std::endl;
            }
            else {
                idx = (idx - 1) / 2;
                std::cout << "Ок" << std::endl;
            }
            break;
        case Command::Влево: {
            int l = 2 * idx + 1;
            if (l >= static_cast<int>(heap.size())) {
                std::cout << "Ошибка! Отсутствует левый потомок" << std::endl;
            }
            else {
                idx = l;
                std::cout << "Ок" << std::endl;
            }
            break;
        }
        case Command::Вправо: {
            int r = 2 * idx + 2;
            if (r >= static_cast<int>(heap.size())) {
                std::cout << "Ошибка! Отсутствует правый потомок" << std::endl;
            }
            else {
                idx = r;
                std::cout << "Ок" << std::endl;
            }
            break;
        }
        case Command::Выход:
            std::cout << "Путешествие по пирамиде закончилось, досвидания" << std::endl;
            return;
        case Command::Ошибка:
            std::cout << "Ошибка! Неизвестная команда" << std::endl;
            break;
        }
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<std::vector<int>> test_cases = {
     {1, 3, 6, 5, 9, 8},
     {94, 67, 18, 44, 55, 12, 6, 42},
     {16, 11, 9, 10, 5, 6, 8, 1, 2, 4}
    };

    std::cout << "Выберите тестовый массив (1, 2, 3): ";
    int sel;
    std::cin >> sel;
    if (sel < 1 || sel > static_cast<int>(test_cases.size())) sel = 1;
    const std::vector<int>& heap = test_cases[sel - 1];

    std::cout << "Исходный массив: ";
    for (int i = 0; i < static_cast<int>(heap.size()); ++i) {
        std::cout << heap[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Пирамида:" << std::endl;
    print_pyramid(heap);

    travel_pyramid(heap);
    return 0;
}

