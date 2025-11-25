#include <iostream>
#include <array>
#include <vector>

// Ќаблюдатель, один класс, закоторым наблюдаем, он содержит список наблюдателей , возможность добавить и удалить и вызвать изменение,
// универсальный класс наблюдателей там метод который мен€ет данные,  потом частные подклассы, которые реализуют изменени€ под себ€ 
// и реализуем общую функцию 



using namespace std;

class Observer
{
public:
    virtual void update(double temperature, double humidity, double pressure) = 0;
};

class WeatherStation
{
private:
    double __temperature, __humidity, __pressure;
    vector<Observer*> __observers; //массив наблюдателей

public:
    void add_observers(Observer* m_observ) { // добавление наблюдател€
        __observers.push_back(m_observ);
    }

    void remove_observers(Observer* m_observ) { // добавление наблюдател€
        auto it = remove(__observers.begin(), __observers.end(), m_observ); // переместим в конец
        __observers.erase(it, __observers.end());
    }

    void notify() {
        for (Observer* obs : __observers) { // проходит по каждому и вызывает изменени€ 
            obs->update(__temperature, __humidity, __pressure);
        }
    }

    void set_measurment(double temperature, double humidity, double pressure)
    {
        __temperature = temperature;
        __humidity = humidity;
        __pressure = pressure;
        notify();
    }
};

class Display : public Observer
{
public:
    void update(double temperature, double humidity, double pressure) override
    {
        cout << "Temperature: " << temperature << "C\n";
        cout << "Humidity: " << humidity << "%\n";      // »справлено (было temperature)
        cout << "Pressure: " << pressure << "mm\n";      // »справлено (было temperature)
    }
};

class Log : public Observer
{
public:
    void update(double temperature, double humidity, double pressure) override
    {
        cout << "To log: " << temperature << " " << "Humidity: " << humidity << " " << "Pressure: " << endl;  

    }
};

class AvgTemp : public Observer
{
    double sum_temp = 0;
    double N_meas = 1;
public:
    void update(double temperature, double humidity, double pressure) override
    {
        sum_temp += temperature;
        cout << "Temp " << sum_temp / N_meas++  << endl;  

    }
};


int main3() {
    setlocale(LC_ALL, "rus");
    WeatherStation WS;
    auto D = new Display();
    auto L = new Log();
    auto A = new AvgTemp();

    WS.add_observers(D);
    WS.add_observers(L);
    WS.add_observers(A);

    WS.set_measurment(10, 70, 650);
    WS.set_measurment(45, 75, 550);
    WS.set_measurment(140, 12, 740);
    WS.set_measurment(5, 5, 450);
    WS.remove_observers(L);
    WS.set_measurment(140, 12, 740);


    return 0;
}