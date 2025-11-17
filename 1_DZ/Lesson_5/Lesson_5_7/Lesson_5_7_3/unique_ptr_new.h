#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <iostream>
#include <utility>
#include <stdexcept>

template<typename T>
class unique_ptr {
private:
    T* ptr;

public:
    // Конструктор с параметром по умолчанию
    explicit unique_ptr(T* p = nullptr) : ptr(p) {
        std::cout << "unique_ptr создан с указателем " << ptr << std::endl;
    }

    // Запрещаем конструктор копирования и оператор присваивания
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    // Конструктор перемещения
    unique_ptr(unique_ptr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
        std::cout << "unique_ptr перемещён, новый адрес: " << ptr << std::endl;
    }

    // Оператор присваивания с перемещением
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Перегрузка оператора *
    T& operator*() const {
        if (ptr == nullptr) {
            throw std::runtime_error("Попытка разыменования nullptr");
        }
        return *ptr;
    }

    // Перегрузка оператора ->
    T* operator->() const {
        return ptr;
    }

    // Метод release
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        std::cout << "Владение освобождено, возвращён указатель: " << temp << std::endl;
        return temp;
    }

    // Метод reset
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    // Метод get
    T* get() const {
        return ptr;
    }

    // Оператор bool
    explicit operator bool() const {
        return ptr != nullptr;
    }

    // Деструктор
    ~unique_ptr() {
        delete ptr;
        std::cout << "unique_ptr уничтожен, память освобождена" << std::endl;
    }
};

#endif // UNIQUE_PTR_H

