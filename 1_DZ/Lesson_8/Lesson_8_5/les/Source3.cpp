#include <iostream>
#include <vector>
#include <algorithm> // Для std::swap
#include <unordered_map> // Включен в оригинальном коде, но не используется в этом примере.


//медиатор - взаимодействие между классами для их распределения
using namespace std;

class Plane
{
public:
    std::string __name;
    virtual void request_take_off() = 0;
    virtual void request_landing() = 0;
    virtual void send_message(const std::string& message) = 0;

    // Рекомендуется добавить виртуальный деструктор для интерфейсов
    virtual ~Plane() = default;
};

class AirControl
{
public:
    virtual void request_take_off(Plane* plane) = 0; // взлет
    virtual void request_landing(Plane* plane) = 0; // пасадка

    // Рекомендуется добавить виртуальный деструктор для интерфейсов
    virtual ~AirControl() = default;
};

class TransportPlane : public Plane
{
    AirControl* __mediator;
    


public:

    // Исправлено: добавлена инициализация 
    TransportPlane(AirControl* mediator, std::string name)
        : __mediator(mediator)
    {
        __name = name;
    }

    void request_take_off() override
    {
        if (__mediator) {
            __mediator->request_take_off(this);
        }
    }

    void request_landing() override
    {
        if (__mediator) {
            __mediator->request_landing(this);
        }
    }

    // Исправлено: send_massage -> send_message
    void send_message(const std::string& message) override
    {
        std::cout << "Transport plane " << __name << " " << message << std::endl;
    }
};

class AirControlTower : public AirControl // Исправлено: добавлено public
{
public:
    void request_take_off(Plane* plane) override
    {
        plane->send_message("Requesting take off...");
        // Логика диспетчера: уведомляем самолет
        if (plane -> __name == "AN-124") {
            std::cout << "Requesting OKKK " << std::endl;
        }
        else
        {
            std::cout << "Requesting NOOOO " << std::endl;
        }
       
    }

    void request_landing(Plane* plane) override
    {
        plane->send_message("Landing landing...");
        // Логика диспетчера: уведомляем самолет
        if (plane->__name != "AN-124") {
            std::cout << "Landing NOOO " << std::endl;
        }
        else
        {
            std::cout << "Landing NOOOO " << std::endl;
        }
    }
};

// запрашивает разрешение на вылет и передает себя

int main() {

    AirControl* tower = new AirControlTower();
    Plane* plane1 = new TransportPlane(tower, "AN-124");
    Plane* plane2 = new TransportPlane(tower, "An-200");
    plane1->request_landing();
    plane1->request_take_off();

    return 0;
}