#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class TcpClient
{
private:
    WSAData _wsa_data;          // Данные библиотеки Winsock
    SOCKET _socket;             // Дескриптор сокета
    sockaddr_in _server_addr;   // Адрес сервера
    char* _buffer;              // Буфер для приема данных
    int _port;                  // Порт сервера
    std::string _server_ip;     // IP-адрес сервера

public:
    // Конструктор - инициализирует и подключает к серверу
    TcpClient();

    // Деструктор - очищает ресурсы
    ~TcpClient();

    // Основной метод - отправляет текст и получает ответ
    void send_text();
};