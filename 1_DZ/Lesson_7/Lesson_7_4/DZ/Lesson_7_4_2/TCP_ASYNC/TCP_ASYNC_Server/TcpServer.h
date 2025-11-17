#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <future>    // Для std::promise, std::future и std::async
#include <vector>    // Для хранения futures
#include <atomic>    // Для потокобезопасного флага

#pragma comment(lib, "Ws2_32.lib")

class TcpServer
{
private:
    WSAData _wsa_data;                   // Данные библиотеки Winsock
    SOCKET _listen_socket;               // Сокет для прослушивания подключений
    sockaddr_in _server_addr;            // Адрес сервера
    int _port;                           // Порт сервера
    std::atomic<bool> _is_running;       // Флаг работы сервера (атомарный для потокобезопасности)

    // Вектор для хранения futures от асинхронных задач
    std::vector<std::future<void>> _client_futures;

    // Метод для обработки отдельного клиента - теперь принимает promise
    void handle_client(SOCKET client_socket, std::promise<std::string> result_promise);

public:
    // Конструктор - инициализирует сервер
    TcpServer();

    // Деструктор - очищает ресурсы
    ~TcpServer();

    // Основной метод - запускает сервер
    void start_listening();

    // Остановка сервера
    void stop_server();
};
