#include <iostream>
#include "TcpClient.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Запуск TCP-клиента..." << std::endl;

    TcpClient client;
    client.send_text();

    std::cout << "Нажмите Enter для выхода..." << std::endl;
    std::cin.get();

    return 0;
}