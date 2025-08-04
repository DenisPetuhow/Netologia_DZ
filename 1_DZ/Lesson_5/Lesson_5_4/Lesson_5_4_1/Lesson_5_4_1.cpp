#include <iostream>
#include <vector>
#include "square.h"

// Статическая вспомогательная функция для вывода вектора
// static означает, что функция имеет внутреннюю связь (internal linkage)
// и доступна только в пределах данного файла
static void printVector(const std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

int main() {
    // Тестирование с простыми числами
    int num1 = 4;
    double num2 = -5.5;

    std::cout << ": " << num1 << std::endl;
    std::cout << ": " << square(num1) << std::endl;
    std::cout << ": " << num2 << std::endl;
    std::cout << ": " << square(num2) << std::endl;

    // Тестирование с вектором
    std::vector<int> vec = { -1, 4, 8 };
    std::vector<int> squared_vec = square(vec);

    std::cout << ": ";
    printVector(vec);
    std::cout << ": ";
    printVector(squared_vec);

    return 0;
}
