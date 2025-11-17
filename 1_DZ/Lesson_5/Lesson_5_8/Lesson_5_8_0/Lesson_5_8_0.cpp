
#include <iostream>
#include "qniq.h"

class A {

    int __value;

public:
    A(int n) : __value(n){}
    int print() {
        return __value;
    }
};

int g_nam = 10000;
int foo() {
    return g_nam;
}

int& foo2() {
    return g_nam;
}

void foo3(int&& r_ref) { // принимает правостороннее значение которое уже имеет адресс
    std::cout << r_ref << " " << &r_ref << std::endl;
}

struct s {
    int x1;
    int x2;
};

struct s2 {
    int x1 : 16; //битовое пространство 1 переменная занимает 2 байта
    int x2 : 16;
};

class B {
public:
    void method() {
        std::cout << "B" << std::endl;
    }

    void method() const{
        std::cout << "B_const" << std::endl;
    }
};

B bar() {
    return B();
}

const B c_bar()  {
    return B();
}

//-------------------------------------------

int g_v = 42;
int&& f1() {
    return std::move(g_v); // Без move не работает так как функция возвращает ссылку на rvalue
}

int&& f2() {
    return 42; // Без move не работает так как функция возвращает ссылку на rvalue
}

int f3() {
    return 120; // Без move не работает так как функция возвращает ссылку на rvalue
}

int& f4() {
    return g_v; // Без move не работает так как функция возвращает ссылку на rvalue
} 

///_________________________________пРАВИЛО 5_________________________________

class Test {
    int* __ptr = nullptr;

public:
    Test() { //Нулевой конструктор
        std::cout << __FUNCSIG__ << std::endl;
    }
    ~Test() { __ptr = nullptr; } // Деструктор

    Test(const Test& T) { // Конструктор копирования
        __ptr = T.__ptr;
        std::cout << __FUNCSIG__ << std::endl;
    }

    void operator=(const Test& T) { // Оператор присваивания
        __ptr = T.__ptr;
        std::cout << __FUNCSIG__ << std::endl;
    }

    Test(Test&& T) noexcept { // Конструктор перемещения (noexcept в функциях принимающих &&
        __ptr = T.__ptr;
        T.__ptr = nullptr;
        std::cout << __FUNCSIG__ << std::endl;
    }

    void operator=(Test&& T) { // Оператор перемещения
        __ptr = T.__ptr;
        T.__ptr = nullptr;
        std::cout << __FUNCSIG__ << std::endl;
    }
};
int main()
{
    int var; // lvalue
    var = 10; //10 rvalue
    
    std::cout << var << std::endl;
    std::cout << &var << std::endl;
    std::cout << 10 << std::endl;
    //std::cout << &10 << std::endl; // ошибка так как у rvalue нет адресса

    A obj = A(120); // lvalue rvalue
    int a = obj.print();
    std::cout << a << std::endl;

    int b = foo();
    std::cout << b << std::endl;
    //foo() = 333; //ошибка так как функция rvalue так как возвращает тип rvalue
    foo2() = 333; //нет ошибка так как функция lvalue так как возвращает тип lvalue (ссылку на переменную не адресс)
    std::cout << g_nam << std::endl;

    
    int aa = 22;
    std::cout << aa <<  &aa << std::endl;
    int* pr = &aa;
    std::cout << pr << std::endl;
    *pr = 55;
    std::cout << aa << &aa << std::endl;
    
    s str;
    std::cout << &str.x1 << ' ' << & str.x2 << std::endl;
    s2 str2;

    int Arr[3]{ 1, 2, 4 };
    int* ptr2 = Arr; // Указатель на первый элемент
    *(ptr2 + 1) = 100; // Записываем именно в переменную 
    std::cout << ptr2[1] << ' '  << ptr2 + 1 <<  std::endl;
    //std::cout << &str2.x1 << ' ' << &str2.x2 << std::endl; // ощибка так как нельзя получать адресс битового поля, получается 2 адресс сылается на часть 1 адресса


    const B c;
    c.method(); // разная реализация если const
    bar().method();
    c_bar().method();

    var = 10;
    //foo3(var) // ошибка так как функция ждет на прием правосторонее значение а мы передаем лево сторонее
    foo3(std::move(var));


    //__________________________________сылки-------------------------
    qniq ptr;
    ptr.main1();  // корректный вызов

    std::cout << f1() << std::endl;
    int&& ff = f1();
    ff = 45555;
    std::cout << ff << " " << &ff << " " << g_v << std::endl; //Взяли адрес локалной ссылки и теперь значение g_v тоже поменялось

    auto&& v1 = f2();  //Универсальная ссылка
    auto&& v2 = f3();
    auto&& v3 = f4();
    std::cout << v1 << " " << v2 << " " << v3 << std::endl; // Так превращаются в лок переменную и все
    std::cout << &v1 << " " << &v2 << " " << &v3 << std::endl;

    decltype(auto) rv1 = f2();
    decltype(auto) rv2 = f3();
    decltype(auto) rv3 = f4(); // Более правильно определяет тип переменной
     
    ///_________________________________пРАВИЛО 5_________________________________

    std::cout << "five" << std::endl;
    Test T1;
    Test T2(T1);
    Test T3;
    T3 = T1;
    Test T4(std::move(T3));
    Test T5;
    T5 = std::move(T1);

    return 0;
}

