

#include <iostream>
#include <thread>
#include <mutex>

// Класс для защищённых данных
class Data {
private:
    int value;      // Скалярные данные (пример)
    std::mutex mtx; // Мьютекс для синхронизации доступа

public:
    // Конструктор для инициализации значения
    explicit Data(int val = 0) : value(val) {}

    // Метод для безопасного чтения значения
    int getValue() const {
        return value;
    }

    // Метод для безопасной установки значения
    void setValue(int val) {
        value = val;
    }

    // Дружественные функции для доступа к мьютексу
    friend void swap_lock(Data& a, Data& b);
    friend void swap_scoped_lock(Data& a, Data& b);
    friend void swap_unique_lock(Data& a, Data& b);
};

// 1. Реализация через std::lock с ручным управлением мьютексами
void swap_lock(Data& a, Data& b) {
    // Захватываем оба мьютекса одновременно (избегаем deadlock)
    std::lock(a.mtx, b.mtx);

    // Используем lock_guard для автоматического освобождения мьютексов
    // std::adopt_lock указывает, что мьютексы уже захвачены
    std::lock_guard<std::mutex> lock_a(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(b.mtx, std::adopt_lock);

    // Выполняем обмен данными
    int temp = a.value;
    a.value = b.value;
    b.value = temp;

    // Мьютексы автоматически освободятся при разрушении lock_guard
}

// 2. Реализация через std::scoped_lock (C++17)
void swap_scoped_lock(Data& a, Data& b) {
    // scoped_lock автоматически захватывает все мьютексы в конструкторе
    // и освобождает в деструкторе. Безопасен относительно deadlock-ов
    std::scoped_lock lock(a.mtx, b.mtx);

    // Обмен данными
    std::swap(a.value, b.value);

    // Мьютексы автоматически освободятся при выходе из области видимости
}

// 3. Реализация через std::unique_lock
void swap_unique_lock(Data& a, Data& b) {
    // unique_lock предоставляет гибкое управление мьютексом
    // std::defer_lock откладывает захват мьютекса
    std::unique_lock<std::mutex> lock_a(a.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(b.mtx, std::defer_lock);

    // Одновременный захват двух мьютексов
    std::lock(lock_a, lock_b);

    // Обмен данными
    int temp = a.value;
    a.value = b.value;
    b.value = temp;

    // Мьютексы освободятся при разрушении unique_lock
}

// Пример использования
int main() {
    setlocale(LC_ALL, "Russian");
    Data d1(42);
    Data d2(123);

    std::cout << "До обмена:\n";
    std::cout << "d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << "\n";

    // Тестируем разные реализации
    swap_lock(d1, d2);
    std::cout << "После swap_lock:\n";
    std::cout << "d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << "\n";

    swap_scoped_lock(d1, d2);
    std::cout << "После swap_scoped_lock:\n";
    std::cout << "d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << "\n";

    swap_unique_lock(d1, d2);
    std::cout << "После swap_unique_lock:\n";
    std::cout << "d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << "\n";

    return 0;
}