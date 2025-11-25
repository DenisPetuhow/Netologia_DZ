#include <iostream>
#include <vector>

// парен command
// создаем класс действие, через него определяем подклассы возмжных действий , который содержит разные объекты
// Создаем общий управленческий класс, содержащий последовательнность этих действий


using namespace std;

class ElectronicDevice  // выполняют одно действие Command, но в зависимости от объекта разная последовательность 
{
private:
    string __name;
public:
    ElectronicDevice(string name) : __name(name) {}
    void turn_on()
    {
        cout << __name << " is now ON" << endl;
    }
    void turn_off()
    {
        cout << __name << " is now OFF" << endl;
    }
};

class Command
{
public:
    virtual void execute() = 0;
};

//Спец интер для запуска методов

class TurnOnCommand : public Command
{
    ElectronicDevice& __device;

public:
    TurnOnCommand (ElectronicDevice& m_device) : __device(m_device) {}
    void execute() override {
        __device.turn_on();
     }
};

class TurnOffCommand : public Command
{
    ElectronicDevice& __device;

public:
    TurnOffCommand(ElectronicDevice& m_device) : __device(m_device) {}
    void execute() override {
        __device.turn_off();
    }
};

class RemoteControll //как пульт, который определяет какие есть возможности
{
    vector<Command*> __buttons;
public:
    void add_command(Command* m_cmd) {
        __buttons.push_back(m_cmd);
    }

    void press_command(int m_slot) { //Нажатие на кнопку выполняется одинаково
        if (m_slot >= 0 && m_slot < __buttons.size()) {
            __buttons[m_slot]->execute();
        }

    }
};


int main1() {
    ElectronicDevice TV("TV");
    ElectronicDevice Radio("Radio");
    RemoteControll remote;
    remote.add_command(new TurnOnCommand(TV));
    remote.add_command(new TurnOnCommand(Radio));
    remote.add_command(new TurnOffCommand(TV));
    remote.add_command(new TurnOffCommand(Radio));

    remote.press_command(0);
    remote.press_command(2);
    remote.press_command(1);
    remote.press_command(3);



    return 0;
}