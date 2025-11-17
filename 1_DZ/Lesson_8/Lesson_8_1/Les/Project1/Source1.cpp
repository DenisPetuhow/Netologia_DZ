#include <iostream>
#include <functional>
#include <memory>

// Определения классов Button, TextBox и их наследников (без изменений)
class Button {
public:
    virtual void render() const noexcept = 0;
    virtual void onClick(std::function<void()>) const noexcept = 0;
    virtual ~Button() = default;
};

class WinButton : public Button {
public:
    void render() const noexcept override {
        std::cout << "Windows Button" << std::endl;
    }
    void onClick(std::function<void()>) const noexcept override {}
};

class MacOSButton : public Button {
public:
    void render() const noexcept override {
        std::cout << "MacOS Button" << std::endl;
    }
    void onClick(std::function<void()>) const noexcept override {}
};

class TextBox {
public:
    virtual void render() const noexcept = 0;
    virtual ~TextBox() = default;

    void setText(const std::string& text) noexcept {
        this->text = text;
    }
    std::string text;
};

class WinText : public TextBox {
public:
    void render() const noexcept override {
        std::cout << "This is windows style text: " << text << std::endl;
    }
};

class MacOSText : public TextBox {
public:
    void render() const noexcept override {
        std::cout << "This is MacOS style text: " << text << std::endl;
    }
};

class AbstractModalDialogFactory {
public:
    virtual Button* createButton() const noexcept = 0;
    virtual TextBox* createTextBox() const noexcept = 0;
    virtual ~AbstractModalDialogFactory() = default;
};

class WindowsModalDialogFactory : public AbstractModalDialogFactory {
public:
    Button* createButton() const noexcept override {
        return new WinButton();
    }
    TextBox* createTextBox() const noexcept override {
        return new WinText();
    }
};

class MacOSModalDialogFactory : public AbstractModalDialogFactory {
public:
    Button* createButton() const noexcept override {
        return new MacOSButton();
    }
    TextBox* createTextBox() const noexcept override {
        return new MacOSText();
    }
};

// ИСПРАВЛЕННЫЙ класс Application
class Application {
public:
    explicit Application(AbstractModalDialogFactory* factory) : factory_(factory) {
        if (factory_) {  // проверка на nullptr
            render();
        }
    }

    void render() const {  // убрал noexcept, т.к. работа с памятью
        Button* b = factory_->createButton();
        TextBox* t = factory_->createTextBox();

        if (t) {
            t->setText("Hello world");
            t->render();
        }

        if (b) {
            b->render();
        }

        // Освобождение памяти!
        delete b;
        delete t;
    }

private:
    AbstractModalDialogFactory* factory_;
};

// ИСПРАВЛЕННЫЙ main
int main2() {
    std::string platform;
    std::cout << "Enter target platform:" << std::endl;
    std::cin >> platform;

    AbstractModalDialogFactory* factory = nullptr;  // инициализация!

    if (platform == "win") {
        factory = new WindowsModalDialogFactory();
    }
    else if (platform == "macos") {
        factory = new MacOSModalDialogFactory();
    }
    else {
        std::cout << "Unknown platform!" << std::endl;
        return 1;  // выход при неизвестной платформе
    }

    Application app(factory);  // теперь должно работать

    delete factory;  // освобождение памяти фабрики

    return 0;
}