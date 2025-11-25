#include <iostream>
#include <string>
#include <algorithm> // Для std::remove
#include <vector>
#include <fstream>

//Один объект (LogSubject) генерирует события (Warning, Error, Fatal). Множество других объектов (Observers) подписываются
//на него и реагируют только на то, что им интересно.

// --- Интерфейс Наблюдателя ---
class Observer {
public:
    virtual ~Observer() = default;
    // Виртуальные методы с пустой реализацией.
    // Наследники переопределяют только то, что им нужно слушать.
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

// --- Наблюдаемый класс (Subject) ---
class LoggerSubject {
private:
    // Список указателей на наблюдателей.
    std::vector<Observer*> observers;
public:
    // Добавление подписчика
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    // Удаление подписчика
    void removeObserver(Observer* observer) {
        // Стандартная идиома erase-remove для вектора
        auto it = std::remove(observers.begin(), observers.end(), observer);
        observers.erase(it, observers.end());
    }

    void warning(const std::string& m_messege) const {
        for (auto* obs : observers) {
            obs->onWarning(m_messege);
        }
    }
    void error(const std::string& m_messege) const {
        for (auto* obs : observers) {
            obs->onError(m_messege);
        }
    }
    void fatalError(const std::string& m_messege) const {
        for (auto* obs : observers) {
            obs->onFatalError(m_messege);
        }
    }
};

// --- Наблюдатель 1: Warning (в консоль) ---
class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "[Observer Warning] Console: " << message << std::endl;
    }
};

// --- Наблюдатель 2: Error (в файл) ---
class ErrorObserver : public Observer {
    std::string filePath;
public:
    ErrorObserver(const std::string& path) : filePath(path) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "[Observer Error] File: " << message << std::endl;
        }
    }
};

// --- Наблюдатель 3: Fatal (Консоль + Файл) ---
class FatalObserver : public Observer {
    std::string filePath;
public:
    FatalObserver(const std::string& path) : filePath(path) {}

    void onFatalError(const std::string& message) override {
        // Печать в консоль
        std::cout << "[Observer Fatal] ALERT: " << message << std::endl;

        // Печать в файл
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "[Observer Fatal] ALERT: " << message << std::endl;
        }
    }
};


int main1() {
    LoggerSubject subject;

    WarningObserver warnObs;
    ErrorObserver errObs("DZ_2_err.txt");
    FatalObserver fatalObs("DZ_2_fatal.txt");
    // Подписываемся
    subject.addObserver(&warnObs);
    subject.addObserver(&errObs);
    subject.addObserver(&fatalObs);

    // Событие Warning (среагирует только warnObs)
    subject.warning("This is a warning");
    // Событие Fatal (среагируют все, у кого метод не пустой, в нашем случае fatalObs)
    subject.error("SYSTEM FAILURE");
    subject.warning("Warning 2 (Should not appear)");
    // Демонстрация удаления
    subject.removeObserver(&warnObs);
    subject.warning("Warning 2 (Should not appear)"); // Никто не отреагирует
    std::cout << "\n";
}

/*
LoggerSubject: Не знает, кто именно его слушает и что они будут делать. Он просто бежит по списку и кричит: "У меня ошибка!".
removeObserver: Критически важен. Если объект наблюдателя (ErrorObserver) будет удален из памяти, но останется в списке observers, программа упадет при попытке вызвать метод у мертвого объекта.
Гибкость: Мы можем добавить нового наблюдателя (например, отправка СМС при FatalError), не меняя код класса LoggerSubject.
*/