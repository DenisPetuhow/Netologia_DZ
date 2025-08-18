#include <iostream>
#include <vector>
#include <memory>
#include <iomanip> // для форматированного вывода
#include <Windows.h>

class tridiagonal_matrix {
public:
    std::vector<double> m_down;     // Нижняя диагональ (a_i, размер n-1)
    std::vector<double> m_upper;    // Верхняя диагональ (c_i, размер n-1)
    std::vector<double> m_middle;   // Главная диагональ (b_i, размер n)

    tridiagonal_matrix(
        const std::vector<double>& down,
        const std::vector<double>& upper,
        const std::vector<double>& middle) :
        m_down{ down }, m_upper{ upper }, m_middle{ middle } {
    };

    ~tridiagonal_matrix() { std::cout << "destructor called\n"; }

    // Метод для создания копии объекта
    std::unique_ptr<tridiagonal_matrix> clone() {
        return std::make_unique<tridiagonal_matrix>(m_down, m_upper, m_middle);
    }

    // Метод для печати матрицы
    void print(const std::string& name) const {
        std::cout << "Трёхдиагональная матрица '" << name << "':\n";
        size_t n = m_middle.size();

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                double value = 0.0;
                if (i == j) {
                    value = m_middle[i];
                }
                else if (i == j + 1) {
                    value = m_down[j];
                }
                else if (i + 1 == j) {
                    value = m_upper[i];
                }
                std::cout << std::setw(6) << value << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
};

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    auto down = std::vector<double>{ -4.0, 5.0 };
    auto upper = std::vector<double>{ 14.0, 8.0 };
    auto middle = std::vector<double>{ 3.0, 1.0, 7.0 };

    // Создаем оригинальную матрицу
    auto matrix = std::make_unique<tridiagonal_matrix>(down, upper, middle);
    matrix->print("Оригинал");

    // Создаем клон матрицы
    auto matrix_clone = matrix->clone();
    matrix_clone->print("Клон");

    return 0;
}