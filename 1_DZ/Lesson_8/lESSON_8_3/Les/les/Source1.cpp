#include <iostream>
#include <vector>
#include <algorithm>

// парен итератор
// создаем свой контейнер с возможностью перехода по его елементам


using namespace std;
template <typename T>
class Container
{
    T* __array = nullptr;
    size_t __size = 0;
public:
    Container(size_t size) : __size(size)
    {
        __array = new T[__size];
    }
    ~Container()
    {
        delete[] __array;
    }
    T& operator[](size_t index)
    {
        return __array[index];
    }

    class Iterator // двигатся по контейнеру
    {
        T* __ptr;
    public:
        Iterator(T* m_ptr) : __ptr(m_ptr) {};

        T& operator*() const { //оператор разоменования
            return *__ptr;
        }

        Iterator& operator++()  { //оператор ++
            ++__ptr;
            return *this;
        }

        Iterator& operator--() { //оператор --
            --__ptr; ++__ptr;        // 1. Сдвигаем внутренний указатель на следующий элемент массива.

            return *this;   // 2. Возвращаем САМОГО СЕБЯ (текущий объект-итератор).
        }

        bool operator!=(const Iterator& other) const {
            return __ptr != other.__ptr;
        }

        bool operator<(const Iterator& other) const {
            return __ptr < other.__ptr;
        }
    };

   /*template <typename Self>
    auto begin(this Self&& self) {
        return Iterator(self.__array);
    }

    template <typename Self>
    auto end(this Self&& self) {
        return Iterator(self.__array + self.__size);
    }*/ // с 23 позволяет работать с const вместе


    // Возвращает итератор на начало массива
    Iterator begin() {
        return Iterator(__array);
    }

    // Возвращает итератор на "конец" (элемент СРАЗУ ЗА последним)
    Iterator end() {
        return Iterator(__array + __size);
    }
};




int main2() {
    setlocale(LC_ALL, "rus");
    vector<int> V;
    vector<int>::iterator I = V.begin(); //создали итератор и поставили его в начало, iterator - класс в нутри класса вектора, декомпозиция, вне класса использоватся не может

    Container<int> Vector(10);
    for (size_t i = 0; i < 10; i++) {
        Vector[i] =  i + 1;
    }

    for (auto it : Vector) {
        cout << it << ' ';
    }

    for (Container<int>::Iterator itt = Vector.begin(); itt < Vector.end(); ++itt){
        cout << *itt << ' ';
    }

    for_each(Vector.begin(), Vector.end(), [](int it) {cout << it << " "; });




    return 0;
}