#include "MyVector.h"

// Конструктор по умолчанию - создаёт пустой вектор
template<typename T>
MyVector<T>::MyVector() : data(nullptr), _size(0), _capacity(0) {
}

// Деструктор - освобождает выделенную память
template<typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}

// Конструктор копирования - создаёт копию другого вектора
template<typename T>
MyVector<T>::MyVector(const MyVector& other) : _size(other._size), _capacity(other._capacity) {
    if (_capacity > 0) {
        data = new T[_capacity];
        // Копируем все элементы
        for (size_t i = 0; i < _size; ++i) {
            data[i] = other.data[i];
        }
    }
    else {
        data = nullptr;
    }
}

// Оператор присваивания
template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {  // Проверка самоприсваивания
        delete[] data;     // Освобождаем старую память

        _size = other._size;
        _capacity = other._capacity;

        if (_capacity > 0) {
            data = new T[_capacity];
            // Копируем все элементы
            for (size_t i = 0; i < _size; ++i) {
                data[i] = other.data[i];
            }
        }
        else {
            data = nullptr;
        }
    }
    return *this;
}

// Доступ к элементу по индексу с проверкой границ
template<typename T>
T& MyVector<T>::at(size_t index) {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Константная версия at()
template<typename T>
const T& MyVector<T>::at(size_t index) const {
    if (index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Добавление элемента в конец вектора
template<typename T>
void MyVector<T>::push_back(const T& value) {
    // Если места недостаточно, увеличиваем буфер
    if (_size >= _capacity) {
        resize_buffer();
    }

    // Добавляем новый элемент
    data[_size] = value;
    ++_size;
}

// Возвращает текущее количество элементов
template<typename T>
size_t MyVector<T>::size() const {
    return _size;
}

// Возвращает текущую ёмкость (сколько элементов помещается без перевыделения)
template<typename T>
size_t MyVector<T>::capacity() const {
    return _capacity;
}

// Приватная функция для увеличения размера буфера
template<typename T>
void MyVector<T>::resize_buffer() {
    size_t new_capacity;

    // Если ёмкость была 0, устанавливаем 1, иначе удваиваем
    if (_capacity == 0) {
        new_capacity = 1;
    }
    else {
        new_capacity = _capacity * 2;  // Стратегия удвоения размера
    }

    // Выделяем новый буфер
    T* new_data = new T[new_capacity];

    // Копируем существующие элементы в новый буфер
    for (size_t i = 0; i < _size; ++i) {
        new_data[i] = data[i];
    }

    // Освобождаем старый буфер
    delete[] data;

    // Обновляем указатель и ёмкость
    data = new_data;
    _capacity = new_capacity;
}

// Явная инстанциация шаблона для часто используемых типов
template class MyVector<int>;
template class MyVector<double>;
template class MyVector<std::string>;