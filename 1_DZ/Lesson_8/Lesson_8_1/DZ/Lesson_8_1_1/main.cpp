#include "sql_query_builder.h" 
#include <iostream>            
#include <cassert>             // Для проверки (assert)

int main() {
    // 1. Создаем объект-строитель
    SqlSelectQueryBuilder query_builder;

    // 2. Настраиваем его с помощью "цепочки" вызовов
    query_builder.AddColumn("name").AddColumn("phone"); // Добавили две колонки
    query_builder.AddFrom("students");                 // Указали таблицу
    query_builder.AddWhere("id", "42").AddWhere("name", "John"); // Добавили два условия

    // 3. Получаем (строим) финальный запрос
    std::string query = query_builder.BuildQuery();

    // Ожидаемый результат, как в задании
    const std::string expected_query = "SELECT name, phone FROM students WHERE id=42 AND name=John;";

    // Выводим результат в консоль, чтобы посмотреть
    std::cout << "Generated query: " << query << std::endl;
    std::cout << "Expected query:  " << expected_query << std::endl;

    // --- Проверка ---
    // В твоем примере был static_assert - он проверяет на этапе компиляции.
    // Но наш query_builder.BuildQuery() работает во время *выполнения* программы.
    // Поэтому мы используем обычный assert (или простой if) для проверки.
    // assert(условие) - если условие ложно, программа аварийно завершится.
    assert(query == expected_query);
    std::cout << "Test 1 PASSED!" << std::endl;

    // --- Тест 2: Проверка на "SELECT *" ---
    SqlSelectQueryBuilder query_builder_all;
    query_builder_all.AddFrom("teachers"); // Колонки не добавляем!
    query_builder_all.AddWhere("subject", "Math");

    std::string query_all = query_builder_all.BuildQuery();
    const std::string expected_all = "SELECT * FROM teachers WHERE subject=Math;";

    std::cout << "\nGenerated query (all): " << query_all << std::endl;
    assert(query_all == expected_all);
    std::cout << "Test 2 (SELECT *) PASSED!" << std::endl;

    // --- Тест 3: Проверка на перезапись AddFrom ---
    SqlSelectQueryBuilder query_builder_from;
    query_builder_from.AddFrom("old_table");
    query_builder_from.AddFrom("new_table"); // Должен перезаписать "old_table"
    query_builder_from.AddColumn("test");

    std::string query_from = query_builder_from.BuildQuery();
    const std::string expected_from = "SELECT test FROM new_table;";

    std::cout << "\nGenerated query (from): " << query_from << std::endl;
    assert(query_from == expected_from);
    std::cout << "Test 3 (AddFrom overwrite) PASSED!" << std::endl;

    return 0;
}