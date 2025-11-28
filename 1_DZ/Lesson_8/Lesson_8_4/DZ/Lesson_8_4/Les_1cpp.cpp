#include <fstream>

class Printable
{
public:
    virtual ~Printable() = default;

    virtual std::string printAsHTML() const = 0;
    virtual std::string printAsText() const = 0;
    virtual std::string printAsJSON() const = 0;
};

class Data : public Printable
{
public:
    enum class Format
    {
        kText,
        kHTML,
        kJSON
    };

    Data(std::string data, Format format)
        : data_(std::move(data)), format_(format) {
    }

    std::string printAsHTML() const override
    {
        if (format_ != Format::kHTML) {
            throw std::runtime_error("Invalid format!");
        }
        return "<html>" + data_ + "<html/>";
    }
    std::string printAsText() const override
    {
        if (format_ != Format::kText) {
            throw std::runtime_error("Invalid format!");
        }
        return data_;
    }
    std::string printAsJSON() const override
    {
        if (format_ != Format::kJSON) {
            throw std::runtime_error("Invalid format!");
        }
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
    Format format_;
};

void saveTo(std::ofstream& file, const Printable& printable, Data::Format format)
{
    switch (format)
    {
    case Data::Format::kText:
        file << printable.printAsText();
        break;
    case Data::Format::kJSON:
        file << printable.printAsJSON();
        break;
    case Data::Format::kHTML:
        file << printable.printAsHTML();
        break;
    }
}

void saveToAsHTML(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kHTML);
}

void saveToAsJSON(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kJSON);
}

void saveToAsText(std::ofstream& file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kText);
}

/*разделения интерфейса.проблема что класс Printable содержит несколько методов для работы с разными типами, в результате получается обобщенный класс интерфейс,
который массивный и влечет необходимость переноса в дочерние классы не нужных методов
Интерфейс Printable слишком «жирный». Если мы создадим класс, который должен поддерживать только текстовый формат, нас все равно заставят реализовать методы printAsHTML 
и printAsJSON, даже если они нам не нужны (придется оставлять заглушки или выбрасывать исключения). Клиенты не должны зависеть от методов, 
которые они не используют.
*/

/*открытости/закрытости, что есть функция saveTo которая содержит виды форматов, которые определены внутри класса, и при добавлении нового типа
* придется изменять сам класс, а лучше создать подклассы для каждого типа формата, и по необходимости создавать новый класс для формата
* Нарушение проявляется в использовании enum и switch-case в функции saveTo и внутри методов класса Data. Чтобы добавить новый формат (например, XML), придется:
Добавить значение в enum Format.
Изменить интерфейс Printable.
Изменить реализацию в Data.
Добавить case в функцию saveTo.
Код должен быть открыт для расширения (создание новых форматтеров), но закрыт для модификации (не нужно трогать существующий код).
*/

/* подстановки Барбары Лисков, что существует единный класс Data, который совмещает несколько типов форматов, лучше было создать один общий, а потом 
* дочерние классы, и передовать в функцию сылку родительского класса, для автоопределения типа формата и уже выполнения print в зависимости от него
такде Нарушение LSP состоит не просто в том, что класс один, а в том, как ведут себя его методы.
Суть нарушения: Интерфейс Printable дает контракт: «Вызови printAsHTML(), и я верну строку». Но реализация Data нарушает этот контракт: 
если формат — kText, то вызов printAsHTML() выбросит исключение std::runtime_error.
Согласно LSP, наследники (реализации) не должны усиливать предусловия. Код, работающий с базовым типом Printable, не должен падать 
с ошибкой при вызове методов интерфейса.

*/