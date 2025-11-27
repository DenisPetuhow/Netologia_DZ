#include <fstream>
#include <string>
#include <stdexcept>
#include <utility>

// 1. Класс Data теперь отвечает ТОЛЬКО за данные (SRP).
// Он больше не знает о том, как его будут печатать.
class Data
{
public:
    explicit Data(std::string data)
        : data_(std::move(data)) {
    }

    const std::string& getData() const {
        return data_;
    }

private:
    std::string data_;
};

// 2. Создаем интерфейс для Форматтера.
// Это решает проблему ISP (один метод) и OCP (можно добавлять новые форматтеры без изменения кода).
class Formatter
{
public:
    virtual ~Formatter() = default;
    virtual std::string format(const Data& data) const = 0;
};

// 3. Реализация конкретных форматтеров.
// Это решает проблему LSP: каждый класс гарантированно возвращает строку в своем формате без исключений.

class HTMLFormatter : public Formatter
{
public:
    std::string format(const Data& data) const override
    {
        return "<html>" + data.getData() + "<html/>";
    }
};

class TextFormatter : public Formatter
{
public:
    std::string format(const Data& data) const override
    {
        return data.getData();
    }
};

class JSONFormatter : public Formatter
{
public:
    std::string format(const Data& data) const override
    {
        return "{ \"data\": \"" + data.getData() + "\"}";
    }
};

// 4. Общая функция сохранения.
// Она не использует switch/case, а полагается на полиморфизм (OCP).
void saveTo(std::ofstream& file, const Data& data, const Formatter& formatter)
{
    file << formatter.format(data) << std::endl;;
}

// Вспомогательные функции, которые требует задание.
// Теперь они создают нужный форматтер и передают его в общую функцию.

void saveToAsHTML(std::ofstream& file, const Data& data) {
    HTMLFormatter formatter;
    saveTo(file, data, formatter);
}

void saveToAsJSON(std::ofstream& file, const Data& data) {
    JSONFormatter formatter;
    saveTo(file, data, formatter);
}

void saveToAsText(std::ofstream& file, const Data& data) {
    TextFormatter formatter;
    saveTo(file, data, formatter);
}

// Пример использования (для проверки, в домашку можно не включать main)

int main() {
    std::ofstream file("out.txt");
    Data myData("Hello SOLID");

    saveToAsHTML(file, myData);
    saveToAsJSON(file, myData);

    return 0;
}
/*
* SRP (Единственная ответственность): Data хранит строку, Formatter форматирует. Раньше Data делал всё сразу.

OCP (Открытость/Закрытость): Если тебе нужно добавить формат XML, ты просто создаешь class XMLFormatter : public Formatter. Тебе не нужно менять класс Data или переписывать switch в функции saveTo.

LSP (Подстановка Лисков): Любой Formatter можно передать в функцию saveTo, и она отработает корректно. Никаких throw exception внутри метода форматирования.

ISP (Разделение интерфейса): Интерфейс Formatter имеет только один метод format. Реализации не обязаны тащить за собой методы других форматов.
*/