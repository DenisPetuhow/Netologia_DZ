#include <iostream>
#include <string>
#include <Windows.h>

// Функция наивного хеширования строки
int simple_string_hash(std::string s) {
    int hash = 0;
    for (char c : s) {
        hash += static_cast<int>(c);
    }
    return hash;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::string input;
    do {
        std::cout << "Введите строку: ";
        std::getline(std::cin, input);
        int hash = simple_string_hash(input);
        std::cout << "Наивный хэш строки " << input << " = " << hash << std::endl;
    } while (input != "exit");

    return 0;
}
