#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <type_traits>

/**
 * @brief Исключение для ошибок парсинга INI файла
 */
class IniParseException : public std::runtime_error {
private:
    size_t line_number_;

public:
    // Конструкторы с учетом правила пяти
    explicit IniParseException(const std::string& message, size_t line = 0);
    IniParseException(const IniParseException& other) = default;
    IniParseException(IniParseException&& other) noexcept = default;
    IniParseException& operator=(const IniParseException& other) = default;
    IniParseException& operator=(IniParseException&& other) noexcept = default;
    virtual ~IniParseException() = default;

    size_t get_line_number() const noexcept { return line_number_; }
};

/**
 * @brief Исключение для случаев когда значение не найдено
 */
class ValueNotFoundException : public std::runtime_error {
private:
    std::vector<std::string> available_keys_;

public:
    // Конструкторы с учетом правила пяти
    explicit ValueNotFoundException(const std::string& message,
        const std::vector<std::string>& available_keys = {});
    ValueNotFoundException(const ValueNotFoundException& other) = default;
    ValueNotFoundException(ValueNotFoundException&& other) noexcept = default;
    ValueNotFoundException& operator=(const ValueNotFoundException& other) = default;
    ValueNotFoundException& operator=(ValueNotFoundException&& other) noexcept = default;
    virtual ~ValueNotFoundException() = default;

    const std::vector<std::string>& get_available_keys() const noexcept {
        return available_keys_;
    }
};

/**
 * @brief Парсер INI файлов
 * Поддерживает секции, переменные, комментарии
 * Использует move-семантику и правило пяти
 */
class IniParser {
private:
    // Контейнер для хранения данных: секция -> (ключ -> значение)
    std::map<std::string, std::map<std::string, std::string>> sections_;
    std::string filename_;

    // Вспомогательные методы для парсинга
    std::string trim(const std::string& str) const;
    bool is_section_line(const std::string& line) const;
    bool is_comment_line(const std::string& line) const;
    bool is_key_value_line(const std::string& line) const;

    std::string extract_section_name(const std::string& line) const;
    std::pair<std::string, std::string> extract_key_value(const std::string& line) const;

    void parse_file();

public:
    // Конструкторы с учетом правила пяти
    explicit IniParser(const std::string& filename);
    explicit IniParser(std::string&& filename);

    // Копирующие конструктор и оператор присваивания
    IniParser(const IniParser& other);
    IniParser& operator=(const IniParser& other);

    // Перемещающие конструктор и оператор присваивания (move-семантика)
    IniParser(IniParser&& other) noexcept;
    IniParser& operator=(IniParser&& other) noexcept;

    // Деструктор
    ~IniParser() = default;

    /**
     * @brief Шаблонная функция для получения значения по ключу
     * @tparam T Тип возвращаемого значения
     * @param key Ключ в формате "section.variable"
     * @return Значение приведенное к типу T
     * @throws ValueNotFoundException если ключ не найден
     * @throws std::invalid_argument если не удается привести к типу T
     */
    template<typename T>
    T get_value(const std::string& key) const;

    // Геттеры для отладки и тестирования
    const std::map<std::string, std::map<std::string, std::string>>& get_sections() const noexcept {
        return sections_;
    }

    const std::string& get_filename() const noexcept {
        return filename_;
    }

private:
    // Вспомогательные шаблонные функции для конвертации типов
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
        convert_value(const std::string& str) const;

    template<typename T>
    typename std::enable_if<std::is_same<T, std::string>::value, T>::type
        convert_value(const std::string& str) const;
};

// Реализация шаблонных методов в заголовочном файле
template<typename T>
T IniParser::get_value(const std::string& key) const {
    // Разделяем ключ на секцию и переменную
    auto dot_pos = key.find('.');
    if (dot_pos == std::string::npos) {
        throw std::invalid_argument("Key must be in format 'section.variable'");
    }

    std::string section = key.substr(0, dot_pos);
    std::string variable = key.substr(dot_pos + 1);

    // Ищем секцию
    auto section_it = sections_.find(section);
    if (section_it == sections_.end()) {
        throw ValueNotFoundException("Section '" + section + "' not found");
    }

    // Ищем переменную в секции
    const auto& variables = section_it->second;
    auto var_it = variables.find(variable);
    if (var_it == variables.end()) {
        // Формируем список доступных ключей для подсказки
        std::vector<std::string> available_keys;
        std::transform(variables.begin(), variables.end(),
            std::back_inserter(available_keys),
            [](const auto& pair) { return pair.first; });

        throw ValueNotFoundException("Variable '" + variable + "' not found in section '" + section + "'",
            std::move(available_keys));
    }

    // Конвертируем значение к нужному типу
    return convert_value<T>(var_it->second);
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
IniParser::convert_value(const std::string& str) const {
    try {
        if constexpr (std::is_integral<T>::value) {
            if constexpr (std::is_same<T, bool>::value) {
                // Обработка булевых значений
                std::string lower_str = str;
                std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
                if (lower_str == "true" || lower_str == "1") return true;
                if (lower_str == "false" || lower_str == "0") return false;
                throw std::invalid_argument("Cannot convert '" + str + "' to bool");
            }
            else {
                return static_cast<T>(std::stoll(str));
            }
        }
        else {
            return static_cast<T>(std::stold(str));
        }
    }
    catch (const std::exception& e) {
        throw std::invalid_argument("Cannot convert '" + str + "' to requested type: " + e.what());
    }
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value, T>::type
IniParser::convert_value(const std::string& str) const {
    return str;
}

#endif // INI_PARSER_H