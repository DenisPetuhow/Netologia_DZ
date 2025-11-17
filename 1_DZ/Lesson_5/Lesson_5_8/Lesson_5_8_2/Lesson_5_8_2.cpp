#include <iostream>
#include <vector>
#include <string>

class big_integer {
private:
    std::vector<int> digits; // Храним цифры в векторе (младший разряд в начале)

public:
    // Конструктор по умолчанию
    big_integer() : digits(1, 0) {}

    // Конструктор из строки
    big_integer(const std::string& str) {
        // Записываем цифры в обратном порядке (младший разряд первый)
        for (int i = str.length() - 1; i >= 0; --i) {
            digits.push_back(str[i] - '0');
        }
    }

    // Конструктор перемещения
    big_integer(big_integer&& other) noexcept : digits(std::move(other.digits)) {
        // После перемещения оставляем корректное состояние
        other.digits = { 0 };
    }

    // Перемещающий оператор присваивания
    big_integer& operator=(big_integer&& other) noexcept {
        if (this != &other) {
            digits = std::move(other.digits);
            other.digits = { 0 };
        }
        return *this;
    }

    // Оператор сложения
    big_integer operator+(const big_integer& other) const {
        big_integer result;
        result.digits.clear();

        int carry = 0; // Перенос
        size_t max_size = std::max(digits.size(), other.digits.size());

        // Складываем поразрядно
        for (size_t i = 0; i < max_size || carry; ++i) {
            int sum = carry;

            // Добавляем цифру из первого числа, если она есть
            if (i < digits.size()) {
                sum += digits[i];
            }

            // Добавляем цифру из второго числа, если она есть
            if (i < other.digits.size()) {
                sum += other.digits[i];
            }

            result.digits.push_back(sum % 10); // Записываем цифру результата
            carry = sum / 10; // Вычисляем перенос
        }

        return result;
    }

    // Оператор умножения на число
    big_integer operator*(int multiplier) const {
        if (multiplier == 0) {
            return big_integer();
        }

        big_integer result;
        result.digits.clear();

        int carry = 0;

        // Умножаем каждую цифру
        for (int digit : digits) {
            int product = digit * multiplier + carry;
            result.digits.push_back(product % 10);
            carry = product / 10;
        }

        // Добавляем оставшийся перенос
        while (carry > 0) {
            result.digits.push_back(carry % 10);
            carry /= 10;
        }

        return result;
    }

    // Оператор вывода
    friend std::ostream& operator<<(std::ostream& os, const big_integer& num) {
        // Выводим цифры в обратном порядке (старший разряд первый)
        for (int i = num.digits.size() - 1; i >= 0; --i) {
            os << num.digits[i];
        }
        return os;
    }
};

// Пример использования
int main() {
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");
    auto result = number1 + number2;
    std::cout << result << std::endl; // 193099

    auto mult_result = number1 * 5;
    std::cout << mult_result << std::endl;

    auto number3(std::move(number1)); //← operator=(big_integer&&)
    std::cout << "number3  " << number3 << std::endl;
    std::cout << "number1  " << number1 << std::endl;
    big_integer number4();
    big_integer result3 = std::move(number2); // ← Конструктор перемещения

    return 0;
}