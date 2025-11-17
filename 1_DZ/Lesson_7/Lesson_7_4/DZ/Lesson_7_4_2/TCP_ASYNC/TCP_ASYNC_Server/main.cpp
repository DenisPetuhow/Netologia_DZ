#include <iostream>
#include "TcpServer.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "=== ЗАПУСК МНОГОПОТОЧНОГО TCP-СЕРВЕРА ===" << std::endl;


    TcpServer server;

    try {
        // Запускаем основной цикл сервера
        server.start_listening();
    }
    catch (const std::exception& e) {
        std::cout << "Критическая ошибка: " << e.what() << std::endl;
    }

    std::cout << "Сервер завершил работу. Нажмите Enter для выхода..." << std::endl;
    std::cin.get();

    return 0;
}