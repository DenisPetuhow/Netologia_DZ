#pragma once

#include <string>   
#include <vector>
#include <map>
#include <utility> // Для std::pair


class SqlSelectQueryBuilder {

public:

    SqlSelectQueryBuilder() = default;

    SqlSelectQueryBuilder& AddColumn(const std::string& column); // Добавляет колонку для выбора (часть SELECT ...).
    //*@return Ссылку на этот же объект(*this), чтобы можно было вызывать методы "цепочкой".

    SqlSelectQueryBuilder& AddFrom(const std::string& table); // * @brief Указывает таблицу (часть FROM ...).
    //По заданию, этот метод должен ПЕРЕЗАПИСЫВАТЬ предыдущее значение.

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value); // * @brief Добавляет условие (часть WHERE ...).
    //* @note Метод сам создаст строку "column=value".

    std::string BuildQuery() const; //Собирает и возвращает готовый SQL - запрос в виде строки.
    // const' в конце означает, что этот метод "обещает" не изменять *внутреннее состояние объекта(поля m_columns, m_from и т.д.).

    SqlSelectQueryBuilder& AddColumn(const std::vector<std::string>& columns) noexcept;
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept;

protected:

    // Внутренняя структура для хранения WHERE условий
    struct WhereCondition {
        std::string column;
        std::string value;
        std::string op = "="; // По умолчанию оператор "="
    };


    std::vector<std::string> m_columns; // Динамический массив для хранения имен колонок (SELECT 'name', 'phone')
    std::string m_from; // Строка для хранения имени таблицы (FROM 'students')
    std::vector<WhereCondition >  m_where_conditions;


};