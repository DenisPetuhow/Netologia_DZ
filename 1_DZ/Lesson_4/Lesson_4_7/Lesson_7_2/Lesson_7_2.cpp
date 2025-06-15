
#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <Windows.h>

// Функция вычисления хэша строки по "серьёзному" алгоритму
uint64_t real_string_hash(const std::string& str, uint64_t p, uint64_t n) {
    uint64_t hash = 0;
    uint64_t p_power = 1;

    for (size_t i = 0; i < str.length(); i++) {
        // Исправленная версия - один % n в конце достаточен
        hash = (hash + static_cast<uint64_t>(str[i]) * p_power) % n;
        p_power = (p_power * p) ;
    }

    return hash;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    uint64_t p, n;

    std::cout << "=== Калькулятор хэша строк ===" << std::endl;
    std::cout << "Введите простое число p: ";
    std::cin >> p;

    std::cout << "Введите модуль n: ";
    std::cin >> n;

    if (n == 0) {
        std::cout << "Ошибка: модуль не может быть равен 0!" << std::endl;
        return 1;
    }

    if (n < 5) {
        std::cout << "Внимание: малое значение модуля может привести к большому количеству коллизий!" << std::endl;
    }

    std::string input;
    std::cout << "\nВведите строки для вычисления хэша (для выхода введите 'exit'):" << std::endl;

    std::cin.ignore();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        uint64_t hash = real_string_hash(input, p, n);
        std::cout << "Хэш строки \"" << input << "\": " << hash << std::endl;

        if (input == "exit") {
            std::cout << "Программа завершена." << std::endl;
            break;
        }
    }

    return 0;
}
