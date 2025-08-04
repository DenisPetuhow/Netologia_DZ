#ifndef SQUARE_H
#define SQUARE_H

#include <vector>

// Определение основного шаблона в заголовочном файле
template<typename T>
T square(const T& value) {
    return value * value;
}

// Специализация для std::vector с inline
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
