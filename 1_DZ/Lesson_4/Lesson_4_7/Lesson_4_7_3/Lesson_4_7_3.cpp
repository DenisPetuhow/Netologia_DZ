#include <iostream>
#include <string>
#include <Windows.h>

int find_substring_light_rabin_karp(std::string source, std::string substring) {
    if (substring.empty() || substring.length() > source.length()) {
        return -1;
    }

    int source_len = source.length();
    int substring_len = substring.length();

    // Вычисляем хеш для искомой подстроки
    int substring_hash = 0;
    for (int i = 0; i < substring_len; i++) {
        substring_hash += substring[i]; //Ведем поиск кеш через неявное преобрзование в asci cid автоматическое неявное преобразование `char` в `int`, так как `substring_hash` имеет тип `int`.
    }

    // Вычисляем хеш для первого окна в исходной строке
    int window_hash = 0;
    for (int i = 0; i < substring_len; i++) {
        window_hash += source[i]; 
    }

    // Проверяем первое окно
    if (window_hash == substring_hash) {
        // Дополнительная проверка на точное совпадение
        if (source.substr(0, substring_len) == substring) {
            return 0;
        }
    }

    // Скользящее окно по остальной части строки
    for (int i = 1; i <= source_len - substring_len; i++) {
        // Обновляем хеш: убираем предыдущий символ, добавляем новый
        window_hash = window_hash - source[i - 1] + source[i + substring_len - 1];

        // Если хеши совпадают, проверяем точное совпадение
        if (window_hash == substring_hash) {
            if (source.substr(i, substring_len) == substring) {
                return i;
            }
        }
    }

    return -1;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::string source, substring;

    std::cout << "Введите строку, в которой будет осуществляться поиск: ";
    std::getline(std::cin, source);

    while (true) {
        std::cout << "Введите подстроку, которую нужно найти: ";
        std::getline(std::cin, substring);

        int index = find_substring_light_rabin_karp(source, substring);

        if (index != -1) {
            std::cout << "Подстрока " << substring << " найдена по индексу " << index << std::endl;
        }
        else {
            std::cout << "Подстрока " << substring << " не найдена" << std::endl;
        }

        // Если введено "exit", завершаем работу
        if (substring == "exit") {
            break;
        }
    }

    return 0;
}
