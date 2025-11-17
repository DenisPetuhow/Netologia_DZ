// ОДИНОЧКА, есть класс, вне зависимости всегда существовал единственный экземпляр класса, например когда создаем обин файл и он передается последовательно, четкая синтаксическая конструкция 
// ОСТОРОЖНО С МНОГОПОТОЧНОСТЬЮ 
#include "thread"
#include "mutex"
#include "iostream"

class SimpleSingleton {
public:
    // Метод для получения экземпляра
    static SimpleSingleton* GetInstance(int value) {
        if (instance_) {
            return instance_;
        }
        else {
            instance_ = new SimpleSingleton(value);
        }
        return instance_;
    }

    void DoJob() const {
        std::cout << value_ << std::endl;
    }

    // Удаленные конструктор копирования и копирующий оператор присваивания
    SimpleSingleton(const SimpleSingleton& s) = delete;
    void operator=(const SimpleSingleton& s) = delete;

private:
    // Приватный конструктор
    explicit SimpleSingleton(int value) : value_(value) {} // приватный конструктор, из майн взять не сможем, 
    int value_;
    // Статический указатель на единственный экземпляр
    static SimpleSingleton* instance_; //статик общий для всех экземпляров классов, тоесть возможно создать только один раз 
    static std::mutex m;
};

SimpleSingleton* SimpleSingleton::instance_ = nullptr;  // надо сразу инициализировать пустым но вклассе это екльзя делать для стат функ
std::mutex SimpleSingleton::m;

void worker1() {
    SimpleSingleton* s = SimpleSingleton::GetInstance(42);
    s->DoJob();
}
void worker2() {
    SimpleSingleton* s = SimpleSingleton::GetInstance(43);
    s->DoJob();
}

int main() {
    std::thread t1(worker1);
    std::thread t2(worker2);
    t1.join();
    t2.join();
}


// объекты сздаються через конструктор SimpleSingleton (п) и копирующий конструктор  SimpleSingleton а(2)