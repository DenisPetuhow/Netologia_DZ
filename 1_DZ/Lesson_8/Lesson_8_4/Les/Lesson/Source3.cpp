// Подстановки Лисков, когда при палиморфизме не происходило проверка на тип данных

// так как точка возвращает 0 то все рушится, если возвращать 00000001 то надо вводить поправки на расщет

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo> // Нужен для использования typeid

#define PI 3.14159265358

using namespace std;

/* Проблема общего интерфейсаЮ что происходит перенос не нужных методов в дочер классы
// Базовый абстрактный класс
class Shape
{
public:
    virtual void drowCircle() = 0;
    virtual void drowSquare() = 0;
    virtual void drowRectangel() = 0;
};

// Класс Круг
class Circle : public Shape
{
public:
    virtual void drowCircle() {
    td::cout << "Cicle" << std::endl;
    }
    
    virtual void drowSquare() {};
    virtual void drowRectangel() {};
};
*/
// Интерфейсный класс не общий а раздельный подж определенные задачи 
class ICircle
{
public:
    virtual void drowCircle() = 0;

};

// Класс Круг
class IRect 
{
public:
    virtual void drowRect() = 0;
};

class Circle : public ICircle
{
public:
     void drowCircle() override{
        std::cout << "Cicle" << std::endl;
    }
};

class Rect : public IRect
{
public:
     void drowRect() override {
        std::cout << "Rect" << std::endl;
    }
};





int main()
{
    // Создаем вектор фигур
    vector<Shape*> shapes{ new Circle(20), new Rect(3, 5), new Point };

    // Выводим результат
    cout << calc(shapes) << endl;

    return 0;
}