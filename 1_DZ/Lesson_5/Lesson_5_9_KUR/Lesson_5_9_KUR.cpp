#include "ini_parser.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

/**
 * @brief Создает тестовый INI файл для демонстрации
 */
void create_test_ini_file(const std::string& filename) {
    std::ofstream file(filename);
    file << "; Тестовый INI файл\n"
        << "; комментарий о разделе\n"
        << "[Section1]\n"
        << "var1=5.0 ; иногда допускается комментарий к отдельному параметру\n"
        << "var2=какая-то строка\n"
        << "bool_var=true\n"
        << "int_var=42\n"
        << "\n"
        << "[Section2]\n"
        << "var1=1\n"
        << "var2=значение_2\n"
        << "\n"
        << "; Иногда значения отсутствуют, это означает, что в Section3 нет переменных\n"
        << "[Section3]\n"
        << "Mode=\n"
        << "Vid=\n"
        << "\n"
        << "; Секции могут повторяться\n"
        << "[Section1]\n"
        << "var3=значение\n"
        << "var1=1.0 ; переприсваиваем значение\n";
}

/**
 * @brief Демонстрирует различные способы использования парсера
 */
void demonstrate_parser_usage() {
    const std::string test_file = "test.ini";

    try {
        // Создаем тестовый файл
        create_test_ini_file(test_file);
        std::cout << "Создан тестовый файл: " << test_file << "\n\n";

        // Создаем парсер (демонстрация move-семантики)
        std::string filename = test_file;
        IniParser parser(std::move(filename)); // filename теперь пуст

        std::cout << "=== Демонстрация работы парсера ===\n";

        // Тестируем различные типы данных
        try {
            auto float_val = parser.get_value<double>("Section1.var1");
            std::cout << "Section1.var1 (double): " << float_val << "\n";

            auto string_val = parser.get_value<std::string>("Section1.var2");
            std::cout << "Section1.var2 (string): " << string_val << "\n";

            auto bool_val = parser.get_value<bool>("Section1.bool_var");
            std::cout << "Section1.bool_var (bool): " << std::boolalpha << bool_val << "\n";

            auto int_val = parser.get_value<int>("Section1.int_var");
            std::cout << "Section1.int_var (int): " << int_val << "\n";

            // Демонстрация переприсваивания значения
            auto overridden_val = parser.get_value<double>("Section1.var1");
            std::cout << "Section1.var1 после переприсваивания: " << overridden_val << "\n";

            auto section2_val = parser.get_value<int>("Section2.var1");
            std::cout << "Section2.var1 (int): " << section2_val << "\n";

            // Пустое значение
            auto empty_val = parser.get_value<std::string>("Section3.Mode");
            std::cout << "Section3.Mode (пустое): '" << empty_val << "'\n";

        }
        catch (const ValueNotFoundException& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }

        std::cout << "\n=== Тестирование обработки ошибок ===\n";

        // Тестируем обработку ошибок
        try {
            // Несуществующая секция
            auto val = parser.get_value<std::string>("NonExistentSection.var");
        }
        catch (const ValueNotFoundException& e) {
            std::cout << "Ошибка (несуществующая секция): " << e.what() << "\n";
        }

        try {
            // Несуществующая переменная с подсказками
            auto val = parser.get_value<std::string>("Section1.nonexistent_var");
        }
        catch (const ValueNotFoundException& e) {
            std::cout << "Ошибка (несуществующая переменная): " << e.what() << "\n";
        }

        try {
            // Неправильная конвертация типа
            auto val = parser.get_value<int>("Section1.var2"); // строка в int
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Ошибка (неправильный тип): " << e.what() << "\n";
        }

        std::cout << "\n=== Тестирование копирования и перемещения ===\n";

        // Демонстрация копирования
        IniParser parser_copy = parser;
        auto copied_val = parser_copy.get_value<std::string>("Section1.var3");
        std::cout << "Значение из копии парсера: " << copied_val << "\n";

        // Демонстрация перемещения
        IniParser parser_moved = std::move(parser_copy);
        auto moved_val = parser_moved.get_value<std::string>("Section2.var2");
        std::cout << "Значение из перемещенного парсера: " << moved_val << "\n";

    }
    catch (const IniParseException& e) {
        std::cout << "Ошибка парсинга: " << e.what();
        if (e.get_line_number() > 0) {
            std::cout << " (строка " << e.get_line_number() << ")";
        }
        std::cout << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Общая ошибка: " << e.what() << "\n";
    }
}

/**
 * @brief Тестирует парсер с некорректным файлом
 */
void test_invalid_file() {
    std::cout << "\n=== Тестирование некорректного файла ===\n";

    // Создаем файл с синтаксическими ошибками
    std::ofstream bad_file("bad_test.ini");
    bad_file << "[Section1]\n"           // строка 1
        << "valid_var=value\n"      // строка 2  
        << "invalid line here\n"    // строка 3 - ошибка
        << "another_var=value\n";   // строка 4
    bad_file.close();

    try {
        IniParser bad_parser("bad_test.ini");
    }
    catch (const IniParseException& e) {
        std::cout << "Поймана ошибка парсинга: " << e.what() << "\n";
        std::cout << "Номер строки с ошибкой: " << e.get_line_number() << "\n";
    }
}

int main() {

    SetConsoleCP(1251);         // Для поддержки русских символов в консоли
    SetConsoleOutputCP(1251);
    std::cout << "=== Тестирование парсера INI файлов ===\n\n";

    demonstrate_parser_usage();
    test_invalid_file();

    std::cout << "\n=== Завершение тестирования ===\n";
    return 0;
}
