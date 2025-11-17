
#include <iostream>
#include <utility>
#include "table.h"
#include <Windows.h>

// Статическая вспомогательная функция для вывода размера таблицы
static void printSize(const std::pair<size_t, size_t>& size) {
    std::cout << "Размер таблицы: " << size.first << "x" << size.second << std::endl;
}

// Статическая функция для вывода содержимого таблицы
template<typename T>
static void printTable(const table<T>& tbl) {
    auto size = tbl.Size();
    std::cout << "Содержимое таблицы:" << std::endl;
    for (size_t i = 0; i < size.first; ++i) {
        for (size_t j = 0; j < size.second; ++j) {
            std::cout << tbl[i][j];
            if (j < size.second - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Тестирование с целыми числами
    std::cout << "=== Тестирование с int ===" << std::endl;
    auto test = table<int>(2, 3);

    // Заполняем таблицу значением 4
    test[0][1] = 4;
    test[1][2] = 8;

    std::cout << "Значение test[0][1]: " << test[0][1] << std::endl;
    std::cout << "Значение test[1][2]: " << test[1][2] << std::endl;

    // Выводим размер таблицы
    printSize(test.Size());

    // Выводим всю таблицу
    printTable(test);

    std::cout << "\n=== Тестирование с double ===" << std::endl;
    // Тестирование с дробными числами
    table<double> doubleTable(3, 2);
    doubleTable[0][0] = 3.14;
    doubleTable[1][1] = -2.5;
    doubleTable[2][0] = 1.618;

    std::cout << "Значение doubleTable[0][0]: " << doubleTable[0][0] << std::endl;
    std::cout << "Значение doubleTable[1][1]: " << doubleTable[1][1] << std::endl;

    printSize(doubleTable.Size());
    printTable(doubleTable);

    std::cout << "\n=== Тестирование константной версии ===" << std::endl;
    // Тестирование константной версии
    const table<int> constTable(2, 2);
    std::cout << "Значение constTable[0][0]: " << constTable[0][0] << std::endl;
    printSize(constTable.Size());

    return 0;

}