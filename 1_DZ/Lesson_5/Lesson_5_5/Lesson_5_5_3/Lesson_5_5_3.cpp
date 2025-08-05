#include <iostream>
#include <string>
#include <Windows.h>
#include "MyVector.h"


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Тестирование MyVector с типом int
    std::cout << "=== Тестирование MyVector<int> ===" << std::endl;

    MyVector<int> vec;

    // Проверяем начальное состояние
    std::cout << "Начальный размер: " << vec.size() << std::endl;
    std::cout << "Начальная ёмкость: " << vec.capacity() << std::endl;

    // Добавляем элементы и наблюдаем за изменением ёмкости
    for (int i = 1; i <= 10; ++i) {
        vec.push_back(i * 10);
        std::cout << "После добавления " << i << " элемента: "
            << "size = " << vec.size()
            << ", capacity = " << vec.capacity() << std::endl;
    }

    // Тестируем доступ к элементам
    std::cout << "\nЭлементы вектора:" << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "vec.at(" << i << ") = " << vec.at(i) << std::endl;
    }

    // Тестируем обработку исключений
    std::cout << "\n=== Тестирование исключений ===" << std::endl;
    try {
        std::cout << "Попытка доступа к элементу с индексом 100..." << std::endl;
        vec.at(100);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }

    // Тестирование с другим типом данных
    std::cout << "\n=== Тестирование MyVector<string> ===" << std::endl;

    MyVector<std::string> str_vec;
    str_vec.push_back("Hello");
    str_vec.push_back("World");
    str_vec.push_back("from");
    str_vec.push_back("MyVector");

    std::cout << "Строковый вектор содержит " << str_vec.size() << " элементов:" << std::endl;
    for (size_t i = 0; i < str_vec.size(); ++i) {
        std::cout << i << ": " << str_vec.at(i) << std::endl;
    }

    // Тестирование конструктора копирования
    std::cout << "\n=== Тестирование копирования ===" << std::endl;
    MyVector<int> vec_copy = vec;  // Используем конструктор копирования

    std::cout << "Копия содержит " << vec_copy.size() << " элементов:" << std::endl;
    for (size_t i = 0; i < vec_copy.size(); ++i) {
        std::cout << vec_copy.at(i) << " ";
    }
    std::cout << std::endl;

    return 0;
}