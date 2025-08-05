#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <Windows.h>



int main()
{
    std::string text;
    std::cout << " [IN]: ";
    std::getline(std::cin, text); // Читаем строку с пробелами и записываем в переменную

    std::unordered_map<char, int>frequency; //создаем неупорядочный контейнер
    for (char c : text) { //перебор символов
        frequency[c]++; // считаем значение ключа через итератор, с у нас ключ (буква)
    }

    std::vector<std::pair<int, char>> sortedFreq;
    for (const auto& element : frequency) {
        sortedFreq.emplace_back(element.second, element.first); //частота символ) //emplace_back передает переменные напрямую, без копирования во временные переменные
    }
    std::sort(sortedFreq.begin(), sortedFreq.end(), std::greater<>()); // std::greater<> -сортировка по убыванию

    std::cout << " [OUT]: " << std::endl;
    for (const auto& element : sortedFreq) {
        std::cout << element.second << " : " << element.first << std::endl;
    }

    return 0;
}