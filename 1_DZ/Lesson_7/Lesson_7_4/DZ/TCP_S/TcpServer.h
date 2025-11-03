#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class TcpServer
{
private:
    WSAData _wsa_data;          // Данные библиотеки Winsock
    SOCKET _listen_socket;      // Сокет для прослушивания
    SOCKET _client_socket;      // Сокет для клиента
    sockaddr_in _server_addr;   // Адрес сервера
    char* _buffer;              // Буфер для приема данных
    int _port;                  // Порт сервера

public:
    // Конструктор - инициализирует и запускает сервер
    TcpServer();

    // Деструктор - очищает ресурсы
    ~TcpServer();

    // Основной метод - принимает подключение и обрабатывает данные
    void start_listening();
};