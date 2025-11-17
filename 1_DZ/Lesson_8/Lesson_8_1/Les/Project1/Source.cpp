

#include "iostream"
#include "functional"

//Фабричный метод

//В вашем случае, метод `create_window()` вызывает `create_button()`, который возвращает указатель на базовый класс `Button`. А конкретный объект(`WinButton` или `MacOSButton`) создаётся в производных классах(`WinDialog` и `MacDialog`)
//.Когда вызывается `b- > render()`, вызывается версия метода соответствующего конкретного объекта — это и есть полиморфизм.Он достигается благодаря виртуальным функциям(`virtual`) и динамическому определению вызываемой версии метода во время выполнения программы.
class Button {
public:
    virtual void render() const noexcept = 0;

    virtual void onClick(std::function<void()>) const noexcept = 0;
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

class Dialog
{
public:
    void create_window()
    {
        Button* b = create_button();
        b->render();
        b->onClick([]() {});

    }
    virtual Button* create_button() = 0;
};

class WinDialog : public Dialog
{
public:
    Button* create_button() override
    {
        return new WinButton();
    }
};

class MacDialog : public Dialog
{
public:
    Button* create_button() override
    {
        return new MacOSButton();
    }
};

int main1() {
    Dialog* windows;
#ifdef _WIN32
    windows = new WinDialog();
#else
    windows = new MacDialog();
#endif

    windows->create_window();
    return 0;

}

//здесь вызовется конструктор `WinDialog()`. В C++, при создании объекта через `new`, синтаксис использует вызов конструктора этой класса — это стандартный способ динамического выделения памяти и инициализации объекта.В случае `new WinDialog()`, 
//происходит вызов конструктора по умолчанию(так как вы явно его не определяли, используется встроенный конструктор)

//.После этого вернётся указатель, указывающий на созданный объект в динамической памяти, который затем присваивается переменной `windows` типа `Dialog*`. Такой подход позволяет работать с объектом через указатель на его базовый класс(`Dialog*`), что и демонстрирует полиморфизм.