#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <utility>

// Шаблонный класс для двумерного массива (таблицы)
template<typename T>
class table {
private:
    std::vector<std::vector<T>> data; // Внутреннее представление таблицы как вектор векторов
    size_t rows;    // Количество строк
    size_t cols;    // Количество столбцов

public:
    // Конструктор, принимающий количество строк и столбцов
    table(size_t row_count, size_t col_count)
        : rows(row_count), cols(col_count) {
        // Инициализируем таблицу с заданными размерами
        data.resize(rows);
        for (auto& row : data) {
            row.resize(cols);
        }
    }

    // Неконстантная версия оператора [] для доступа к строке
    std::vector<T>& operator[](size_t row) {
        return data[row];
    }

    // Константная версия оператора [] для доступа к строке
    const std::vector<T>& operator[](size_t row) const {
        return data[row];
    }

    // Константная функция Size, возвращающая размер таблицы как пару (строки, столбцы)
    std::pair<size_t, size_t> Size() const {
        return std::make_pair(rows, cols);
    }


};

#endif // TABLE_H