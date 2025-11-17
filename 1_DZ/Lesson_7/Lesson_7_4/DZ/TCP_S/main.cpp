#include <iostream>
#include "TcpServer.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Запуск TCP-сервера..." << std::endl;

    TcpServer server;
    server.start_listening();

    std::cout << "Сервер завершил работу. Нажмите Enter для выхода..." << std::endl;
    std::cin.get();

    return 0;
}