#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <sstream>

// Функтор для подсчёта суммы и количества чисел, делящихся на 3
class DivisibleByThreeCounter {
private:
    int sum;    // Сумма чисел, делящихся на 3
    int count;  // Количество чисел, делящихся на 3
public:
    // Конструктор по умолчанию инициализирует сумму и количество нулями
    DivisibleByThreeCounter() : sum(0), count(0) {}

    // Оператор, вызываемый для каждого элемента коллекции
    void operator()(int number) {
        if (number % 3 == 0) { // Проверяем, делится ли число на 3 без остатка
            sum += number;     // Если да, добавляем к сумме
            count++;           // И увеличиваем количество
        }
    }

    int get_sum() const { return sum; }     // Геттер для суммы
    int get_count() const { return count; } // Геттер для количества
};

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    std::vector<int> numbers;
    std::string input;

    std::cout << "Введите числа (для завершения введите 'stop'):" << std::endl;

    // Читаем ввод до получения команды "stop"
    while (std::cin >> input) {
        if (input == "stop") {
            break; // Выходим из цикла при получении команды завершения
        }

        // Используем stringstream для преобразования строки в число
        std::stringstream ss(input);
        int num;
        if (ss >> num && ss.eof()) { // Проверяем, что вся строка была преобразована в число 1. `ss >> num` - пытается извлечь число из stringstream и записать его в переменную `num`. Возвращает `true`, если операция прошла успешно (т.е. в начале строки действительно было число).

                                    //2. `ss.eof()` - проверяет, достигнут ли конец потока.Возвращает `true`, если весь поток был прочитан полностью.
            numbers.push_back(num);  // Добавляем число в вектор
        }
        else {
            std::cout << "'" << input << "' не является числом. Попробуйте ещё раз." << std::endl;
        }
    }

    // Выводим полученный массив
    std::cout << "\nПолученный массив: ";
    if (numbers.empty()) {
        std::cout << "(пустой)";
    }
    else {
        for (size_t i = 0; i < numbers.size(); ++i) {
            std::cout << numbers[i];
            if (i < numbers.size() - 1) {
                std::cout << " ";
            }
        }
    }
    std::cout << std::endl;

    // Создаём объект функтора
    DivisibleByThreeCounter counter;

    // Применяем функтор к каждому элементу вектора
    counter = std::for_each(numbers.begin(), numbers.end(), counter);

    // Выводим результаты, как требуется в примере
    std::cout << "get_sum() = " << counter.get_sum() << std::endl;
    std::cout << "get_count() = " << counter.get_count() << std::endl;

    return 0;
}