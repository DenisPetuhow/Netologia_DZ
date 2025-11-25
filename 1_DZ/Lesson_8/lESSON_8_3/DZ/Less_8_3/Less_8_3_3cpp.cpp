#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//nextHandler: Ключевой элемент. Каждый обработчик знает только о том, кто стоит следующим в очереди.
//handle: Содержит логику if (мое) { делай; } else { передай дальше; }.
//Приоритет: Порядок обработки определяется тем, в каком порядке мы соединим объекты в main.


// --- Типы сообщений ---
enum class LogType {
    Warning,
    Error,
    Fatal,
    Unknown,
    AA
};

// --- Класс сообщения ---
class LogMessage {
    LogType _type;
    std::string _message;
public:
    LogMessage(LogType type, const std::string& msg) : _type(type), _message(msg) {}
    LogType type() const { return _type; }
    const std::string& message() const { return _message; }
};

// --- Абстрактный Обработчик ---
class LogHandler {
protected:
    LogHandler* nextobject = nullptr;
public:
    virtual ~LogHandler() = default;

    // Настройка цепочки (возвращаем указатель, чтобы можно было чейнить: set->set->set)
    void setNext(LogHandler* handler) {
        nextobject = handler;
    }

    // Основной метод обработки
    virtual void handle(const LogMessage& msg) = 0;

};

// --- Обработчик Fatal Error ---
class FatalHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == LogType::Fatal) {
            // Логика: выбросить исключение
            throw std::runtime_error("FATAL ERROR PROCESSED: " + msg.message());
        }
        // Если не наш тип, передаем дальше
        else if (nextobject) {
            nextobject->handle(msg);
        }
    }
};

// --- Обработчик Error ---
class ErrorHandler : public LogHandler {
    std::string filePath;
public:
    ErrorHandler(const std::string& path) : filePath(path) {}
    void handle(const LogMessage& msg) override {
        if (msg.type() == LogType::Error) {
            // Печать в файл
            std::ofstream file(filePath, std::ios::app);
            if (file.is_open()) {
                file << "[Chain Error] To File: " << msg.message() << std::endl;

            }
        }
        else if (nextobject) {
            nextobject->handle(msg);
        }
    }
};

// --- Обработчик Warning ---
class WarningHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == LogType::Warning) {

            std::cout << "[Chain Warning] Console: " << msg.message() << std::endl;
        }
        else if (nextobject) {
            nextobject->handle(msg);
        }
    }
};

// --- Обработчик Unknown ---
class UnknownHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == LogType::Unknown) {
            throw std::runtime_error("Unknown message type encountered: " + msg.message());
        }
        else if (nextobject) {
            nextobject->handle(msg);
        }
    }
};

int main2() {
    // Создаем обработчики
    FatalHandler hFatal;
    ErrorHandler hError("DZ_3_err.txt");
    WarningHandler hWarn;
    UnknownHandler hUnknown;

    // Строим цепь: Fatal -> Error -> Warning -> Unknown
    hFatal.setNext(&hError);
    hError.setNext(&hWarn);
    hWarn.setNext(&hUnknown);

    try {
        // Тест 1: Warning (пройдет через Fatal и Error, остановится на Warning)
        hFatal.handle(LogMessage(LogType::Warning, "Low disk space"));

        // Тест 2: Error (пройдет через Fatal, остановится на Error)
        hFatal.handle(LogMessage(LogType::Error, "Connection timeout"));

        // Тест 3: Fatal (сработает сразу на первом звене)
        // Ожидаем исключение
        hFatal.handle(LogMessage(LogType::Fatal, "Core dump!"));
        hFatal.handle(LogMessage(LogType::AA, "Core dump!"));
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    try {
        // Тест 4: Unknown (пройдет всю цепь до конца)
        hFatal.handle(LogMessage(LogType::Unknown, "Alien signal"));
    }
    catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}

/*
 * Пояснение:
nextHandler: Ключевой элемент. Каждый обработчик знает только о том, кто стоит следующим в очереди.
handle: Содержит логику if (мое) { делай; } else { передай дальше; }.
Приоритет: Порядок обработки определяется тем, в каком порядке мы соединим объекты в main.
*/
