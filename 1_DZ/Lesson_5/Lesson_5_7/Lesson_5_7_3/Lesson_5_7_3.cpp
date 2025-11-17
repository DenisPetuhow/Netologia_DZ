
#include <Windows.h>
#include "test_class.h"
#include "unique_ptr_new.h"


int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Создаём unique_ptr с объектом
    unique_ptr<TestClass> ptr1(new TestClass(42));

    // Используем оператор * для доступа к объекту
    (*ptr1).print();

    // Используем оператор -> для доступа к методам
    ptr1->print();

    // Проверяем, что указатель не пустой
    if (ptr1) {
        std::cout << "ptr1 не пустой" << std::endl;
    }

    // Перемещаем владение
    unique_ptr<TestClass> ptr2 = std::move(ptr1);

    // ptr1 теперь пустой
    if (!ptr1) {
        std::cout << "ptr1 теперь пустой" << std::endl;
    }

    // Освобождаем владение и получаем сырой указатель
    TestClass* raw_ptr = ptr2.release();

    // Теперь мы должны вручную удалить объект
    delete raw_ptr;

    return 0;
}
