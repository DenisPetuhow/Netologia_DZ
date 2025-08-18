#include "test_class.h"

TestClass::TestClass(int v) : value(v) {
    std::cout << "TestClass создан со значением " << value << std::endl;
}

TestClass::~TestClass() {
    std::cout << "TestClass со значением " << value << " уничтожен" << std::endl;
}

void TestClass::print() const {
    std::cout << "Значение: " << value << std::endl;
}

int TestClass::getValue() const {
    return value;
}

void TestClass::setValue(int v) {
    value = v;
}