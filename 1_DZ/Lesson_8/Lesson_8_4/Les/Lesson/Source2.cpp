// Подстановки Лисков, когда при палиморфизме не происходило проверка на тип данных

// так как точка возвращает 0 то все рушится, если возвращать 00000001 то надо вводить поправки на расщет

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo> // Нужен для использования typeid

#define PI 3.14159265358

using namespace std;

// Базовый абстрактный класс
class Shape
{
public:
    virtual double calc_area() = 0;
};

// Класс Круг
class Circle : public Shape
{
    double _R_;
public:
    Circle(double R) : _R_(R) {}
    double calc_area() override
    {
        return PI * _R_ * _R_;
    }
};

// Класс Прямоугольник
class Rect : public Shape
{
    double _A_, _B_;
public:
    Rect(double A, double B) : _A_(A), _B_(B) {}
    double calc_area() override
    {
        return _A_ * _B_;
    }
};

// Класс Точка (добавлен для контекста, так как он есть в проверке typeid)
// У точки площадь 0, что вызывает проблему при делении
class Point : public Shape
{
public:
    double calc_area() override
    {
        return 0;
    }
};

// Функция вычисления
double calc(vector<Shape*>& shapes)
{
    double result = 0;
    for (auto& shape : shapes)
    {
        // Проверка из первого скриншота.
        // Это попытка обойти нарушение принципа Liskov:
        // мы проверяем, не является ли фигура "Точкой", прежде чем считать.
        // Обратите внимание: в реальном C++ для проверки типа объекта по указателю
        // часто используют typeid(*shape) или dynamic_cast.
        if (typeid(shape) != typeid(Point*))
        {
            cout << typeid(shape).name(); 
            result += 1 / shape->calc_area();
        }
    }
    return result;
}

int main3()
{
    // Создаем вектор фигур
    vector<Shape*> shapes{ new Circle(20), new Rect(3, 5), new Point };

    // Выводим результат
    cout << calc(shapes) << endl;

    return 0;
}