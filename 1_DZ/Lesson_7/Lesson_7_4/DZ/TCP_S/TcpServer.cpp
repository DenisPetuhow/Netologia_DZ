#include "TcpServer.h"
#include <iostream>
#include <cstdlib>

TcpServer::TcpServer() : _port(12345), _client_socket(INVALID_SOCKET)
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
    _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_socket == INVALID_SOCKET) {
        std::cout << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // 3. Настройка адреса сервера
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_port = htons(_port);
    _server_addr.sin_addr.s_addr = INADDR_ANY;  // Принимаем подключения на все интерфейсы

    // 4. Привязка сокета к адресу
    int bindResult = bind(_listen_socket, (sockaddr*)&_server_addr, sizeof(_server_addr));
    if (bindResult == SOCKET_ERROR) {
        std::cout << "Ошибка привязки сокета: " << WSAGetLastError() << std::endl;
        closesocket(_listen_socket);
        WSACleanup();
        return;
    }

    // 5. Начало прослушивания
    int listenResult = listen(_listen_socket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR) {
        std::cout << "Ошибка прослушивания: " << WSAGetLastError() << std::endl;
        closesocket(_listen_socket);
        WSACleanup();
        return;
    }

    std::cout << "Сервер запущен на порту " << _port << std::endl;
}

TcpServer::~TcpServer()
{
    // Закрытие сокетов и очистка Winsock
    if (_client_socket != INVALID_SOCKET) {
        closesocket(_client_socket);
    }
    if (_listen_socket != INVALID_SOCKET) {
        closesocket(_listen_socket);
    }
    WSACleanup();
    delete[] _buffer;
}

void TcpServer::start_listening()
{
    std::cout << "Ожидание подключений..." << std::endl;

    // 6. Принятие входящего подключения
    _client_socket = accept(_listen_socket, NULL, NULL);
    if (_client_socket == INVALID_SOCKET) {
        std::cout << "Ошибка принятия подключения: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Клиент подключен!" << std::endl;

    // 7. Получение данных от клиента
    int bytesReceived = recv(_client_socket, _buffer, 1024, 0);
    if (bytesReceived > 0) {
        std::cout << "Получено от клиента (" << bytesReceived << " байт): " << _buffer << std::endl;

        // 8. Отправка ответа клиенту
        std::string response = "Сообщение получено сервером!";
        int sendResult = send(_client_socket, response.c_str(), response.length() + 1, 0);
        if (sendResult == SOCKET_ERROR) {
            std::cout << "Ошибка отправки ответа: " << WSAGetLastError() << std::endl;
        }
        else {
            std::cout << "Ответ отправлен клиенту (" << sendResult << " байт)" << std::endl;
        }
    }
    else if (bytesReceived == 0) {
        std::cout << "Клиент отключился" << std::endl;
    }
    else {
        std::cout << "Ошибка приема данных: " << WSAGetLastError() << std::endl;
    }

    // 9. Закрытие клиентского сокета
    closesocket(_client_socket);
    _client_socket = INVALID_SOCKET;
}