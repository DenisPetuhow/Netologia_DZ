// Декоратор, без изменения основного классакода, менять текст визуализацию, исходный объект не изменяется
// изменение скорости работы, например в отделном классе реализовать метод для подсчета выполнения конретной функции родительского класса
//основа, добавляем функционал, не трогая объект 

#include <string>
#include <iostream>

class Text {
public:
    virtual void render(const std::string& data) const {
        std::cout << data;
    }
};


class DecoratedText : public Text {
   
public:
    explicit DecoratedText(Text* text) : text_(text) {}
    Text* text_;  // ЧТОБЫ УСПОЛЬЗОВАТЬ УЖЕ СОЗДАННЫЙ ОБЪЕКТ, А НЕ СОЗДАВАТЬ НОВЫЙ  

};

class ItalicText : private DecoratedText {
public:
    explicit ItalicText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data)  const override {
        std::cout << "<i>";
        text_->render(data);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText {
public:
    explicit BoldText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const {
        std::cout << "<b>";
        text_->render(data);
        std::cout << "</b>";
    }
};

int main2() {
    auto text_block = new ItalicText(new BoldText(new Text())); // последовательно запустить 
    text_block->render("Hello world");

    std::cout << std::endl;
    Text text1;
    text1.render("hello");
    ItalicText i_text(&text1);
    std::cout << std::endl;
    i_text.render("i hello");


    return 0;
}