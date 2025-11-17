#include <iostream>
#include <algorithm>  // для std::move
#include <iterator>   // для std::back_inserter
#include <string>
#include <vector>

template<typename T>
void move_vectors(std::vector<T>& source, std::vector<T>& destination) {
    // Перемещаем элементы из source в конец destination
    std::move(source.begin(), source.end(), std::back_inserter(destination));
    // Очищаем исходный вектор
    source.clear();
}
template<typename T>
void printVector(const std::vector<T>& v) {
    if (!v.empty()) {
        for (const auto& elem : v) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
    else { std::cout << "ERROR" << std::endl; }
}

int main()
{
    std::vector<std::string> one = { "test_string1", "test_string2" };
    std::vector<std::string> two;
    move_vectors(one, two);
    printVector(two);
    printVector(one);

    std::vector<int> a = { 22,44,66,88,55  };
    std::vector<int> b;
    move_vectors(a, b);
    printVector(a);
    printVector(b);



    return 0;
}


