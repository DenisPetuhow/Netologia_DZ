#include <iostream>

#include <vector>
#include <Windows.h>
#include <algorithm>
#include <sstream>

// Функция для удаления всех дубликатов из вектора
void makeUnique(std::vector<int>& vec) {
    // Шаг 1: Сортируем вектор, чтобы одинаковые элементы оказались рядом
    // Это необходимо для корректной работы std::unique
    std::sort(vec.begin(), vec.end());

    // Шаг 2: Используем std::unique для удаления дубликатов
    // std::unique перемещает все уникальные элементы в начало контейнера // и возвращает итератор на первый "мусорный" элемент
    
    auto newEndIT = std::unique(vec.begin(), vec.end());

    // Шаг 3: Удаляем "мусорные" элементы с конца вектора
    // erase удаляет элементы от newEnd до конца вектора
    vec.erase(newEndIT, vec.end());
}


int main()
{
    int size_vec;
    std::cout << " [IN]: ";
    std::cin >> size_vec; // Читаем строку с пробелами и записываем в переменную
    std::vector<int> vec;
    for (size_t i = 0; i < size_vec; ++i) { 
        int num;
        std::cout << " Element_" << i + 1 << "= ";
        std::cin >> num;  // Считываем каждый элемент
        vec.push_back(num);  // Добавляем элемент в вектор
    }

    std::cout << " [OUT] first vec: " << std::endl;
    for (const auto& element : vec) {
        std::cout << "element: " << element << std::endl;
    }

    // Удаляем дубликаты
    makeUnique(vec);
    std::cout << " [OUT] finali vec: " << std::endl;
    for (const auto& element : vec) {
        std::cout << "element: " << element << std::endl;
    }


    return 0;
}