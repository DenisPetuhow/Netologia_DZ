#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// Суть: Мы инкапсулируем действие (в данном случае «печать лога») в объект. Это позволяет нам передавать этот объект
//в функции, не зная заранее, куда именно (в консоль или файл) будет производиться запись.

// --- Интерфейс Команды ---
class LogCommand {
public:
    virtual ~LogCommand() = default;
    // Метод print принимает сообщение. Сама команда знает "как" печатать, но "что" печатать приходит извне.
    virtual void print(const std::string& message) = 0;
};

class ConsleLogCommand : public LogCommand {
public:
    void print(const std::string& m_message) override {
        std::cout << "[Console] " << m_message << std::endl;
    }
};

// --- Конкретная Команда 2: Печать в файл ---
class FileLogCommand : public LogCommand {
private:
    std::string filePath; // Храним состояние: путь к файлу
public:
    // Конструктор принимает путь, так как это настройка самой команды
    FileLogCommand(const std::string& m_path) : filePath(m_path) {}

    void print(const std::string& m_message) override {
        std::ofstream file(filePath, std::ios::app); // Открываем на дозапись
        if (file.is_open()) {
            file << "[File] " << m_message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Error opening file!" << std::endl;
        }
    }
};

void executePrint(LogCommand& m_command, const std::string& m_messeg) {
    m_command.print(m_messeg);
}

int main() {
    ConsleLogCommand consoleCmd;
    FileLogCommand fileCmd("DZ_1_log_cmd.txt");
    executePrint(consoleCmd, "Hello Console");
    executePrint(fileCmd, "Hello File");
    std::cout << "\n";

}

/*
 Пояснение:
LogCommand: Это абстрактный контракт. Любой, кто хочет логировать, обязан уметь делать print.
FileLogCommand: В конструкторе получает path. Это важно: команда "запоминает", с каким ресурсом она работает.
executePrint: Демонстрирует принцип подстановки. Мы можем передать туда объект ConsoleLogCommand или FileLogCommand, код функции при этом не меняется.
*/