#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <stdexcept>

template<typename T>
class MyVector {
private:
    T* data;           // Указатель на массив элементов
    size_t _size;      // Текущее количество элементов
    size_t _capacity;  // Максимальное количество элементов без перевыделения памяти

    // Приватная функция для увеличения размера буфера
    void resize_buffer();

public:
    // Конструктор по умолчанию
    MyVector();

    // Деструктор
    ~MyVector();

    // Конструктор копирования
    MyVector(const MyVector& other);

    // Оператор присваивания
    MyVector& operator=(const MyVector& other);

    // Доступ к элементу по индексу с проверкой границ
    T& at(size_t index);
    const T& at(size_t index) const;

    // Добавление элемента в конец
    void push_back(const T& value);

    // Получение текущего размера
    size_t size() const;

    // Получение текущей ёмкости
    size_t capacity() const;
};

#endif // MYVECTOR_H