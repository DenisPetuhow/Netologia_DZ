#include <string>
#include <iostream>
#include <vector>

//Принцып закр и открытости 

using namespace std;
/* // плохая реализация, так как не можем легко изменить функ
class Animal
{
public:
    std::string m_Name;

    Animal(std::string iName) : m_Name(iName) {}
    std::string GetName() { return m_Name; }
};

// В векторе присутствует "Cat", но логика для него в функции ниже отсутствует
const std::vector<Animal> animals = { Animal("Lion"), Animal("Mouse"), Animal("Snake"), Animal("Cat") };

void AnimalSounds(const std::vector<Animal>& animals)
{
    for (const auto& i : animals)
    {
        if (i.m_Name == "Lion") std::cout << "roar"; else
            if (i.m_Name == "Mouse") std::cout << "squeak"; else
                if (i.m_Name == "Snake") std::cout << "hiss";

        std::cout << std::endl;
    }
}
*/

class Animal
{
public:
    virtual string make_sounds() = 0;

};

class Lion : public Animal
{
    string make_sounds() override {
        return "roar";
    }
};

class Mouse : public Animal
{
    string make_sounds() override {
        return "squeak";
    }
};

class Snake : public Animal
{
    string make_sounds() override {
        return "hiss";
    }
};

// В векторе присутствует "Cat", но логика для него в функции ниже отсутствует
const std::vector<Animal*> animals = { new Lion, new Mouse, new Snake };

void AnimalSounds(const std::vector<Animal*>& animals)
{
    for (const auto& i : animals)
    {
        cout << i->make_sounds();
    }
}


int main2()
{
    AnimalSounds(animals);
    return 0;
}
