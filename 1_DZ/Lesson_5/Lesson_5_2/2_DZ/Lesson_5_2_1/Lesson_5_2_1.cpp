#include <iostream>
#include <stdexcept>

class smart_array {
private:
    int* data;          // Указатель на массив данных
    size_t capacity;    // Максимальное количество элементов
    size_t current_size; // Текущее количество элементов

public:
    // Конструктор, принимающий количество элементов
    smart_array(size_t size) : capacity(size), current_size(0) {
        if (size == 0) {
            throw std::invalid_argument("Array size must be greater than 0");
        }
        data = new int[capacity]; // Выделяем память для массива
    }

    // Деструктор - освобождает выделенную память
    ~smart_array() {
        delete[] data;
    }

    // Запрещаем копирование для простоты (можно реализовать при необходимости)
    smart_array(const smart_array&) = delete;
    smart_array& operator=(const smart_array&) = delete;

    // Функция добавления нового элемента
    void add_element(int element) {
        if (current_size >= capacity) {
            throw std::overflow_error("Array is full, cannot add more elements");
        }
        data[current_size] = element;
        current_size++;
    }

    // Функция получения элемента по индексу
    int get_element(size_t index) const {
        if (index >= current_size) {
            throw std::out_of_range("Index is out of range");
        }
        return data[index];
    }

    // Дополнительная функция для получения текущего размера
    size_t size() const {
        return current_size;
    }

    // Дополнительная функция для получения вместимости
    size_t get_capacity() const {
        return capacity;
    }
};

// Пример использования
int main() {
    try {
        smart_array arr(5);
        arr.add_element(1);
        arr.add_element(4);
        arr.add_element(155);
        arr.add_element(14);
        arr.add_element(15);
        std::cout << arr.get_element(1) << std::endl; // Выведет 4

        // Попытка добавить шестой элемент вызовет исключение
         arr.add_element(100);

        // Попытка получить элемент с неправильным индексом
        // std::cout << arr.get_element(10) << std::endl;

    }
    catch (const std::overflow_error& ex) {
        std::cout << "Переполнение массива: " << ex.what() << std::endl;
    }
    catch (const std::out_of_range& ex) {
        std::cout << "Выход за границы: " << ex.what() << std::endl;
    }
    catch (const std::invalid_argument& ex) {
        std::cout << "Некорректный аргумент: " << ex.what() << std::endl;
    }
    return 0;
}