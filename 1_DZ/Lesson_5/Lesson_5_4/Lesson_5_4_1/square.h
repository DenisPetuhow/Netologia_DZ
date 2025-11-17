#ifndef SQUARE_H
#define SQUARE_H

#include <vector>

// Основной шаблон функции для возведения в квадрат
template<typename T>
T square(const T& value);

// Специализация с inline - предотвращает ошибку множественного определения
template<>
inline std::vector<int> square<std::vector<int>>(const std::vector<int>& vec) {
    std::vector<int> result;
    result.reserve(vec.size());

    for (const int& element : vec) {
        result.push_back(element * element);
    }

    return result;
}

#endif // SQUARE_H