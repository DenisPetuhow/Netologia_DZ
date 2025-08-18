#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <iostream>

class TestClass {
private:
    int value;

public:
    explicit TestClass(int v);
    ~TestClass();

    void print() const;
    int getValue() const;
    void setValue(int v);
};

#endif // TEST_CLASS_H