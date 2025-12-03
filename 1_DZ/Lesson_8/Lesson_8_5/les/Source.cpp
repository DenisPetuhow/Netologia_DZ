#include <iostream>
#include <string>

using namespace std;

class Engine
{
public:
    void start_engine()
    {
        cout << "Engine started " << endl;
    }
    void stop_engine()
    {
        cout << "Engine stopped " << endl;
    }
};

class Electricity
{
public:
    void turn_on()
    {
        cout << "Ele started " << endl;
    }
    void turn_off()
    {
        cout << "Engine stopped " << endl;
    }
};

class Velecity 
{
    Engine __egine;
    Electricity __electric;

public:
    void start_car() {
        __electric.turn_on();
        __egine.start_engine();
    }
};

int main1() {
    Velecity car;
    car.start_car();
    return 0;
}