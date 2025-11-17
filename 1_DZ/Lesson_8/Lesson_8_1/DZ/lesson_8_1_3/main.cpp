#include "sql_query_builder.h" 
#include "advanced_sql_query_builder.h" // Подключаем НОВЫЙ класс
#include <iostream>            
#include <cassert>             // Для проверки (assert)

int main() {

    std::cout << "\n--- Task 3 Tests ---\n";

    AdvancedSqlSelectQueryBuilder advanced_builder;

    // Используем унаследованные методы
    advanced_builder.AddColumn( "name" );
    advanced_builder.AddFrom("students");

    // Используем НОВЫЙ метод
    advanced_builder.AddWhereGreater("id", "42"); // Вместо SomeMethod(...)

    const std::string expected_3 = "SELECT name FROM students WHERE id>42;";
    std::string actual_3 = advanced_builder.BuildQuery(); // Используем унаследованный BuildQuery()

    std::cout << "Generated Query: " << actual_3 << std::endl;
    assert(actual_3 == expected_3);

    // Тест на смешивание
    advanced_builder.AddWhere("name", "Alex"); // Используем старый метод (добавит "AND name=Alex")

    const std::string expected_4 = "SELECT name FROM students WHERE id>42 AND name=Alex;";
    std::string actual_4 = advanced_builder.BuildQuery();

    std::cout << "Generated Query: " << actual_4 << std::endl;
    assert(actual_4 == expected_4);

    std::cout << "Task 3 Tests Passed!" << std::endl;



    return 0;
}