#include "ini_parser.h"
#include <iostream>
#include <cctype>

// Реализация исключений
IniParseException::IniParseException(const std::string& message, size_t line)
    : std::runtime_error(line > 0 ? "Line " + std::to_string(line) + ": " + message : message)
    , line_number_(line) {
}

ValueNotFoundException::ValueNotFoundException(const std::string& message,
    const std::vector<std::string>& available_keys)
    : std::runtime_error([&]() {
    std::string full_message = message;
    if (!available_keys.empty()) {
        full_message += ". Available keys: ";
        for (size_t i = 0; i < available_keys.size(); ++i) {
            if (i > 0) full_message += ", ";
            full_message += available_keys[i];
        }
    }
    return full_message;
        }())
    , available_keys_(available_keys) {
}

// Конструкторы IniParser
IniParser::IniParser(const std::string& filename) : filename_(filename) {
    parse_file();
}

IniParser::IniParser(std::string&& filename) : filename_(std::move(filename)) {
    parse_file();
}

// Копирующий конструктор
IniParser::IniParser(const IniParser& other)
    : sections_(other.sections_), filename_(other.filename_) {
}

// Копирующий оператор присваивания
IniParser& IniParser::operator=(const IniParser& other) {
    if (this != &other) {
        sections_ = other.sections_;
        filename_ = other.filename_;
    }
    return *this;
}

// Перемещающий конструктор
IniParser::IniParser(IniParser&& other) noexcept
    : sections_(std::move(other.sections_)), filename_(std::move(other.filename_)) {
}

// Перемещающий оператор присваивания
IniParser& IniParser::operator=(IniParser&& other) noexcept {
    if (this != &other) {
        sections_ = std::move(other.sections_);
        filename_ = std::move(other.filename_);
    }
    return *this;
}

// Вспомогательные методы
std::string IniParser::trim(const std::string& str) const {
    // Используем STL алгоритмы для удаления пробелов
    auto start = std::find_if_not(str.begin(), str.end(),
        [](unsigned char ch) { return std::isspace(ch); });
    auto end = std::find_if_not(str.rbegin(), str.rend(),
        [](unsigned char ch) { return std::isspace(ch); }).base();

    return start < end ? std::string(start, end) : std::string();
}

bool IniParser::is_section_line(const std::string& line) const {
    std::string trimmed = trim(line);
    return !trimmed.empty() && trimmed.front() == '[' && trimmed.back() == ']';
}

bool IniParser::is_comment_line(const std::string& line) const {
    std::string trimmed = trim(line);
    return !trimmed.empty() && trimmed.front() == ';';
}

bool IniParser::is_key_value_line(const std::string& line) const {
    return line.find('=') != std::string::npos && !is_comment_line(line);
}

std::string IniParser::extract_section_name(const std::string& line) const {
    std::string trimmed = trim(line);
    if (trimmed.length() < 2) {
        throw IniParseException("Invalid section format");
    }

    // Удаляем квадратные скобки и обрезаем пробелы
    return trim(trimmed.substr(1, trimmed.length() - 2));
}

std::pair<std::string, std::string> IniParser::extract_key_value(const std::string& line) const {
    auto eq_pos = line.find('=');
    if (eq_pos == std::string::npos) {
        throw IniParseException("Invalid key-value format");
    }

    std::string key = trim(line.substr(0, eq_pos));
    std::string value_part = line.substr(eq_pos + 1);

    // Удаляем комментарий если есть
    auto comment_pos = value_part.find(';');
    if (comment_pos != std::string::npos) {
        value_part = value_part.substr(0, comment_pos);
    }

    std::string value = trim(value_part);

    if (key.empty()) {
        throw IniParseException("Empty key name");
    }

    return std::make_pair(std::move(key), std::move(value));
}

void IniParser::parse_file() {
    std::ifstream file(filename_);
    if (!file.is_open()) {
        throw IniParseException("Cannot open file: " + filename_);
    }

    std::string line;
    size_t line_number = 0;
    std::string current_section;

    try {
        while (std::getline(file, line)) {
            ++line_number;

            // Пропускаем пустые строки и комментарии
            if (trim(line).empty() || is_comment_line(line)) {
                continue;
            }

            // Обработка секций
            if (is_section_line(line)) {
                current_section = extract_section_name(line);
                // Создаем секцию если её нет (или используем существующую)
                if (sections_.find(current_section) == sections_.end()) {
                    sections_[current_section] = std::map<std::string, std::string>();
                }
            }
            // Обработка переменных
            else if (is_key_value_line(line)) {
                if (current_section.empty()) {
                    throw IniParseException("Variable outside of section", line_number);
                }

                auto key_value = extract_key_value(line);
                // Если переменная уже существует, перезаписываем (как указано в требованиях)
                sections_[current_section][key_value.first] = key_value.second;
            }
            else {
                // Неопознанная строка
                throw IniParseException("Invalid line format", line_number);
            }
        }

    }
    catch (const IniParseException& e) {
        // Пробрасываем исключения парсинга как есть
        throw;
    }
    catch (const std::exception& e) {
        // Оборачиваем другие исключения
        throw IniParseException(std::string("Parse error: ") + e.what(), line_number);
    }
}