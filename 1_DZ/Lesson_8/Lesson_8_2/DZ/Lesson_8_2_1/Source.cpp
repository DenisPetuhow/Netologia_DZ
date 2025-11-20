#include <string>
#include <iostream>
#include <algorithm> // Необходим для std::reverse [19, 20]
#include <memory>    // Для std::unique_ptr (рекомендация Best Practices)


/**
 * 1. Component (Интерфейс)
 * Определяет базовый контракт для всех текстовых объектов.

 */
class Text {
public:
    virtual std::string render() const = 0;
    virtual ~Text() = default;
};

/**
 * 2. Concrete Component (Реализация)
 * Простой объект, хранящий строку. Ядро "луковицы" декораторов.
 */
class SimpleText : public Text {
    std::string text_;
public:
    explicit SimpleText(std::string t) : text_(std::move(t)) {}

    // override обеспечивает проверку сигнатуры компилятором [27]
    std::string render() const override {
        return text_;
    }
};

/**
 * 3. Base Decorator (Базовый класс декоратора)
 * Хранит ссылку на объект Text. Обратите внимание на использование raw pointer
 * для совместимости с условиями задачи, хотя std::unique_ptr был бы безопаснее.
 */
class Decorator : public Text {
protected:
    Text* ptr_; // Указатель на оборачиваемый объект
public:
    explicit Decorator(Text* ptr) : ptr_(ptr) {}

    std::string render() const override {
        return ptr_->render(); // Делегирование вызова
    }
};

/**
 * Decorator 1: Paragraph
 * Оборачивает результат рендеринга в теги <p>.
 */
class Paragraph : public Decorator {
public:
    explicit Paragraph(Text* ptr) : Decorator(ptr) {}

    std::string render() const override {
        return "<p>" + ptr_->render() + "</p>";
    }
};

/**
 * Decorator 2: Reversed
 * Инвертирует строку. Это пример изменения данных, а не структуры.
 */
class Reversed : public Decorator {
public:
    explicit Reversed(Text* ptr) : Decorator(ptr) {}

    std::string render() const override {
        std::string data = ptr_->render();
        std::reverse(data.begin(), data.end());
        return data;
    }
};

/**
 * Decorator 3: Link
 * Пример декоратора с дополнительным состоянием (url).

 */
class Link : public Decorator {
    std::string url_;
public:
    Link(Text* ptr, std::string url) : Decorator(ptr), url_(std::move(url)) {}

    std::string render() const override {
        return "<a href=" + url_ + ">" + ptr_->render() + "</a>";
    }
};
int main() {
    // --- Тест 1: Decorator ---
    std::cout << "=== Decorator Pattern Test ===" << std::endl;

    // Создаем цепочку: Link -> Paragraph -> Reversed -> Text
    // Логика: Сначала текст реверсируется, затем оборачивается в параграф, затем в ссылку.
    // "Hello world" -> "dlrow olleH" -> "<p>dlrow olleH</p>" -> "<a href=...>...</a>"

    Text* text_obj = new SimpleText("Hello world");
    Text* reversed = new Reversed(text_obj);
    Text* paragraph = new Paragraph(reversed);
    Text* link = new Link(paragraph, "netology.ru");

    std::cout << link->render() << std::endl;

    // Очистка памяти (в обратном порядке создания оберток)
    // Примечание: При использовании std::unique_ptr это произошло бы автоматически.
    delete link;
    delete paragraph;
    delete reversed;
    delete text_obj;

    return 0;
}