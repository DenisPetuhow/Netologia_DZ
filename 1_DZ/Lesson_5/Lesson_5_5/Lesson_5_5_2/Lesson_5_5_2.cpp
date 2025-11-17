#include <iostream>
#include <set>
#include <vector>

int main() {

    std::cout << " [IN]: " << std::endl;
    int n;
    std::cin >> n;

    // Используем set с компаратором greater для сортировки по убыванию
    std::set<int, std::greater<int>> uniqueNumbers;

    for (int i = 0; i < n; i++) {
        int number;
        std::cin >> number;
        uniqueNumbers.insert(number);
    }

    std::cout << " [OUT]: " << std::endl;
    // Выводим все числа (они уже отсортированы по убыванию)
    for (const int& num : uniqueNumbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
