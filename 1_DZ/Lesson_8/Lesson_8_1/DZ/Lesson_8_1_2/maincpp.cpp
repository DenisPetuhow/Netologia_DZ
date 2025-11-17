#include "sql_query_builder.h" 
#include <iostream>            
#include <cassert>             // Для проверки (assert)

int main() {
   
    //... (тесты Задачи 1)

    std::cout << "\n--- Task 2 Tests ---\n";
    SqlSelectQueryBuilder query_builder_2;

    // Готовим данные для новых методов
    std::vector<std::string> cols = { "name", "email", "address" };
    std::map<std::string, std::string> wheres = {
        {"city", "New York"},
        {"age", "30"}
    };

    query_builder_2.AddFrom("users")
        .AddColumn(cols) // Используем AddColumns(vector)
        .AddWhere(wheres); // Используем AddWhere(map)

    const std::string expected_2 = "SELECT name, email, address FROM users WHERE age=30 AND city=New York;";
    // ПРИМЕЧАНИЕ: std::map отсортирован по ключу, поэтому 'age' будет раньше 'city'.

    std::string actual_2 = query_builder_2.BuildQuery();

    std::cout << "Generated Query: " << actual_2 << std::endl;
    assert(actual_2 == expected_2);

    std::cout << "Task 2 Tests Passed!" << std::endl;


    return 0;
}