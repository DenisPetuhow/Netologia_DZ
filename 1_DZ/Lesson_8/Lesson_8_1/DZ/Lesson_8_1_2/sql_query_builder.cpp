#include "sql_query_builder.h"

#include <sstream>

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(const std::string& column) {
    // Просто добавляем имя колонки в конец нашего вектора
    m_columns.push_back(column);
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddFrom(const std::string& table) {
    // Как и требовалось в задании, мы просто ПЕРЕЗАПИСЫВАЕМ значение
    m_from = table;

    // Снова возвращаем *this для цепочки
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::string& column, const std::string& value) {
    // Добавляем пару (колонка, значение) в наш вектор
    m_where_conditions.push_back({ column, value });
    return *this; // Возвращаем себя для "цепочки"
}


std::string SqlSelectQueryBuilder::BuildQuery() const {
    std::stringstream ss; // Создаем строковый поток

    // 1. Часть SELECT
    ss << "SELECT ";
    if (m_columns.empty()) {
        // По заданию: если колонки не добавлены, используем "SELECT *"
        ss << "*";
    }
    else {
        // Собираем колонки, избегая "висячей запятой" [12, 13]
        for (size_t i = 0; i < m_columns.size(); ++i) {
            if (i > 0) {
                ss << ", "; // Добавляем запятую ПЕРЕД всеми, кроме первого
            }
            ss << m_columns[i];
        }
    }

    // 2. Часть FROM
    // (Для простоты не проверяем, была ли вызвана AddFrom, 
    // хотя в реальном проекте стоило бы)
    ss << " FROM " << m_from;

    // 3. Часть WHERE
    if (!m_where_conditions.empty()) {
        ss << " WHERE ";
        // Собираем условия, избегая "висячего AND"
        for (size_t i = 0; i < m_where_conditions.size(); ++i) {
            if (i > 0) {
                ss << " AND "; // Добавляем AND ПЕРЕД всеми, кроме первого
            }
            // Формируем условие "key=value"
            ss << m_where_conditions[i].first << "=" << m_where_conditions[i].second;
        }
    }

    // 4. Завершающий символ
    ss << ";"; // По заданию

    return ss.str(); // Возвращаем собранную строку
}

// ---- РЕАЛИЗАЦИЯ ЗАДАЧИ 2 ----

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(const std::vector<std::string>& columns) noexcept {
    // Мы можем просто вставить весь диапазон из входного вектора 
    // в конец нашего внутреннего вектора columns_
    m_columns.insert(m_columns.end(), columns.begin(), columns.end());

    return *this; // Возвращаем себя для "цепочки"
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::map<std::string, std::string>& kv) noexcept {
    // Проходим по каждому элементу в 'map'.
    // 'const auto&' - самый эффективный способ итерации "только для чтения".
    for (const auto& m_pair : kv) {
        // Мы можем либо вызвать наш собственный существующий метод:
        // AddWhere(pair.first, pair.second);

        // Либо, что чуть-чуть быстрее, добавить пару напрямую:
        m_where_conditions.push_back({ m_pair.first, m_pair.second });
    }

    return *this; // Возвращаем себя для "цепочки"
}


