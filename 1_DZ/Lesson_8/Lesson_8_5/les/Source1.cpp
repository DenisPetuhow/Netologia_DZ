#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "Timer.h"

using namespace std;

class Charachter
{
    char __character;
public:
    Charachter(char character) : __character(character) {}
    void draw(int color)
    {
        consol_color::SetColor(color, 0);
        cout << __character;
        consol_color::SetColor(15, 0);
    }
};

class Alphabet
{
    unordered_map<char, Charachter*> __alphabet; // хранит список значение объект
public:
    Charachter* get_char(char key)
    {
        if (__alphabet.find(key) == __alphabet.end()) {
            __alphabet[key] = new Charachter(key); // создаем объект со значнение ключа
        }

        return __alphabet[key];
    }
};


int main2()
{
    Alphabet dic;
    dic.get_char('a')->draw(2);
    dic.get_char('a')->draw(5);
    dic.get_char('b')->draw(2);
    dic.get_char('b')->draw(2);
    dic.get_char('c')->draw(5);
    dic.get_char('c')->draw(12);
    return 0;
}

// каждая конкретная буква не один объект с полем цвет, а это одна исходная буква б но с разным цветом