#include "Client.h"
#include <iostream>
#include <cstdlib>

TcpClient::TcpClient() : _port(12345), _server_ip("127.0.0.1")
{
    // Выделяем память под буфер
    _buffer = new char[1024];

    // 1. Инициализация Winsock
    int err = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
    if (err != 0) {
        std::cout << "Ошибка инициализации Winsock: " << err << std::endl;
        return;
    }

    // 2. Создание TCP-сокета
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == INVALID_SOCKET) {
        std::cout << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // 3. Настройка адреса сервера
    _server_addr.sin_family = AF_INET;                 // IPv4
    _server_addr.sin_port = htons(_port);              // Порт
    _server_addr.sin_addr.s_addr = inet_addr(_server_ip.c_str()); // IP-адрес

    // 4. Подключение к серверу
    int connectResult = connect(_socket, (sockaddr*)&_server_addr, sizeof(_server_addr));
    if (connectResult == SOCKET_ERROR) {
        std::cout << "Ошибка подключения к серверу: " << WSAGetLastError() << std::endl;
        closesocket(_socket);
        WSACleanup();
        return;
    }

    std::cout << "Успешно подключено к серверу " << _server_ip << ":" << _port << std::endl;
}

TcpClient::~TcpClient()
{
    // Закрытие сокета и очистка Winsock
    if (_socket != INVALID_SOCKET) {
        closesocket(_socket);
    }
    WSACleanup();
    delete[] _buffer;
}

void TcpClient::send_text()
{
    // 5. Отправка данных серверу
    std::string message = "Привет, это тестовое сообщение от TCP-клиента!";

    int sendResult = send(_socket, message.c_str(), message.length() + 1, 0);
    if (sendResult == SOCKET_ERROR) {
        std::cout << "Ошибка отправки данных: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Send: " << message.c_str() << "Отправлено " << sendResult << " байт серверу" << std::endl;

    // 6. Получение ответа от сервера
    int bytesReceived = recv(_socket, _buffer, 1024, 0);
    if (bytesReceived > 0) {
        std::cout << "Получено от сервера (" << bytesReceived << " байт): " << _buffer << std::endl;
    }
    else if (bytesReceived == 0) {
        std::cout << "Соединение закрыто сервером" << std::endl;
    }
    else {
        std::cout << "Ошибка приема данных: " << WSAGetLastError() << std::endl;
    }
}