#include <iostream>
#include <array>
#include <vector>

// ѕаттерн Ђ÷епочка ответственностиї, класс родитель, от которого наследуютьс€ другие
// и переопредел€ют метод действи€ и цепочки
using namespace std;

class Authentificator
{
public:
    virtual void handler_request(const string& request) = 0;
    virtual void set_next_handler(Authentificator* handler) = 0;
};

class UsernamePassword : public Authentificator
{
    Authentificator* __next_handler = nullptr;
public:
    void set_next_handler(Authentificator* handler) override {
        __next_handler = handler;
    }
    void handler_request(const string& request) override {
        if (request == "user_pass") {
            cout << "Log and pass" << endl;
        }
        else if (__next_handler != nullptr){
                __next_handler->handler_request(request) ;
        }
        else {
            cout << "ERROR" << endl;
        }
    }
};

class OAuth : public Authentificator
{
    Authentificator* __next_handler = nullptr;
public:
    void set_next_handler(Authentificator* handler) override {
        __next_handler = handler;
    }
    void handler_request(const string& request) override {
        if (request == "OAuth") {
            cout << "OAuth aut" << endl;
        }
        else if (__next_handler != nullptr) {
            __next_handler->handler_request(request);
        }
        else {
            cout << "ERROR" << endl;
        }
    }
};


int main() {
    setlocale(LC_ALL, "rus");
    Authentificator* method1 = new UsernamePassword();
    Authentificator* method2 = new OAuth();

    method1->set_next_handler(method2); // все вопросы через method1
    method1->handler_request("user_pass");
    method1->handler_request("OAuth");
    method1->handler_request("A");


    return 0;
}