#pragma once
// inline-реализация
#include "sql_query_builder.h" // Включаем родительский класс

// Синтаксис наследования:
// class Потомок : public Родитель [18, 21]
class AdvancedSqlSelectQueryBuilder : public SqlSelectQueryBuilder {
public:
    // Новый метод, которого нет у родителя
    AdvancedSqlSelectQueryBuilder& AddWhereOperator(const std::string& col,
        const std::string& val,
        const std::string& op) {

        // Мы ИМЕЕМ ДОСТУП к 'where_conditions_', потому что он 'protected'
        // Мы просто добавляем в него структуру с указанным оператором
        m_where_conditions.push_back({ col, val, op });

        return *this; // Возвращаем себя для "цепочки"
    }

    // "Удобные" обертки для конкретных операторов
    AdvancedSqlSelectQueryBuilder& AddWhereGreater(const std::string& col, const std::string& val) {
        return AddWhereOperator(col, val, ">");
    }

    AdvancedSqlSelectQueryBuilder& AddWhereLess(const std::string& col, const std::string& val) {
        return AddWhereOperator(col, val, "<");
    }
};

