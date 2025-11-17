#include "TcpServer.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>  // Для std::stringstream


TcpServer::TcpServer() : _port(12345), _is_running(false)
{
    // 1. ИНИЦИАЛИЗАЦИЯ WINSOCK
    // WSAStartup должна быть вызвана перед любыми другими Winsock функциями
    int err = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
    if (err != 0) {
        std::cout << "Ошибка инициализации Winsock: " << err << std::endl;
        return;
    }

    // 2. СОЗДАНИЕ СОКЕТА
    // AF_INET - IPv4, SOCK_STREAM - TCP, 0 - протокол по умолчанию
    _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_socket == INVALID_SOCKET) {
        std::cout << "Ошибка создания сокета: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // 3. НАСТРОЙКА ОПЦИИ ПОВТОРНОГО ИСПОЛЬЗОВАНИЯ АДРЕСА
    // Это позволяет перезапускать сервер сразу после закрытия
    int opt = 1;
    if (setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        std::cout << "Ошибка настройки SO_REUSEADDR: " << WSAGetLastError() << std::endl;
    }

    // 4. НАСТРОЙКА АДРЕСА СЕРВЕРА
    _server_addr.sin_family = AF_INET;           // Семейство адресов - IPv4
    _server_addr.sin_port = htons(_port);        // Порт (htons преобразует в сетевой порядок байт)
    _server_addr.sin_addr.s_addr = INADDR_ANY;   // Принимать подключения на все интерфейсы

    // 5. ПРИВЯЗКА СОКЕТА К АДРЕСУ
    // Связываем сокет с IP-адресом и портом
    int bindResult = bind(_listen_socket, (sockaddr*)&_server_addr, sizeof(_server_addr));
    if (bindResult == SOCKET_ERROR) {
        std::cout << "Ошибка привязки сокета: " << WSAGetLastError() << std::endl;
        closesocket(_listen_socket);
        WSACleanup();
        return;
    }

    // 6. НАЧАЛО ПРОСЛУШИВАНИЯ
    // SOMAXCONN - максимальная длина очереди ожидающих подключений
    int listenResult = listen(_listen_socket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR) {
        std::cout << "Ошибка прослушивания: " << WSAGetLastError() << std::endl;
        closesocket(_listen_socket);
        WSACleanup();
        return;
    }

    _is_running = true;
    std::cout << "Сервер запущен на порту " << _port << std::endl;
}

TcpServer::~TcpServer()
{
    stop_server();
}

// МЕТОД ОБРАБОТКИ КЛИЕНТА 
void TcpServer::handle_client(SOCKET client_socket, std::promise<std::string> result_promise)
{
    char buffer[1024];  // Локальный буфер для каждого клиента

    // Получаем информацию о клиенте для логирования
    sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);
    getpeername(client_socket, (sockaddr*)&client_addr, &addr_len);
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    // ИСПРАВЛЕНИЕ: Получаем ID потока как строку, а не через преобразование указателя
    std::stringstream thread_id_stream;
    thread_id_stream << std::this_thread::get_id();
    std::string thread_id_str = thread_id_stream.str();

    std::string client_info = std::string(client_ip) + ":" + std::to_string(ntohs(client_addr.sin_port));
    std::cout << "Начата обработка клиента " << client_info
        << " в потоке " << thread_id_str << std::endl;

    std::string result_message;  // Сообщение-результат, которое вернем через promise
    try {
        // 7. ПОЛУЧЕНИЕ ДАННЫХ ОТ КЛИЕНТА
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // Добавляем нуль-терминатор для безопасности
            std::cout << "Получено от клиента " << client_info << " (" << bytesReceived
                << " байт): " << buffer << std::endl;

            // 8. ОТПРАВКА ОТВЕТА КЛИЕНТУ
            // ИСПРАВЛЕНИЕ: Используем строковое представление ID потока
            std::string response = "Сообщение получено сервером! Обработано в потоке: " + thread_id_str;

            int sendResult = send(client_socket, response.c_str(), response.length() + 1, 0);
            if (sendResult == SOCKET_ERROR) {
                result_message = "Ошибка отправки ответа клиенту " + client_info;
                std::cout << result_message << ": " << WSAGetLastError() << std::endl;
            }
            else {
                result_message = "Успешно обработан клиент " + client_info +
                    " в потоке " + thread_id_str +
                    ", отправлено " + std::to_string(sendResult) + " байт";
                std::cout << result_message << std::endl;
            }
        }
        else if (bytesReceived == 0) {
            result_message = "Клиент " + client_info + " отключился (поток: " + thread_id_str + ")";
            std::cout << result_message << std::endl;
        }
        else {
            result_message = "Ошибка приема данных от клиента " + client_info + " (поток: " + thread_id_str + ")";
            std::cout << result_message << ": " << WSAGetLastError() << std::endl;
        }
    }
    catch (const std::exception& e) {
        result_message = "Исключение при обработке клиента " + client_info +
            " (поток: " + thread_id_str + "): " + e.what();
        std::cout << result_message << std::endl;
    }

    // 9. ЗАКРЫТИЕ КЛИЕНТСКОГО СОКЕТА
    closesocket(client_socket);
    std::cout << "Соединение с клиентом " << client_info << " закрыто (поток: " + thread_id_str + ")" << std::endl;

    // 10. ПЕРЕДАЧА РЕЗУЛЬТАТА ЧЕРЕЗ PROMISE
    result_promise.set_value(result_message);
}

void TcpServer::start_listening()
{
    std::cout << "Сервер начал прослушивание. Ожидание подключений..." << std::endl;

    while (_is_running) {
        // ИСПОЛЬЗУЕМ SELECT ДЛЯ ТАЙМАУТА 
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(_listen_socket, &readSet);

        timeval timeout;
        timeout.tv_sec = 1;  // Таймаут 1 секунда для проверки флага _is_running
        timeout.tv_usec = 0;

        int selectResult = select(0, &readSet, NULL, NULL, &timeout);

        if (selectResult == SOCKET_ERROR) {
            std::cout << "Ошибка в select: " << WSAGetLastError() << std::endl;
            break;
        }

        if (selectResult == 0) {
            // Таймаут - проверяем флаг _is_running и продолжаем цикл
       
            continue;
        }

        // 11. ПРИНЯТИЕ ВХОДЯЩЕГО ПОДКЛЮЧЕНИЯ
        SOCKET client_socket = accept(_listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            if (_is_running) {
                std::cout << "Ошибка принятия подключения: " << WSAGetLastError() << std::endl;
            }
            continue;
        }

        std::cout << "Обнаружено новое подключение! Создание асинхронной задачи..." << std::endl;

        // 12. СОЗДАЕМ PROMISE И FUTURE е
        std::promise<std::string> result_promise;
        std::future<std::string> result_future = result_promise.get_future();

        // 13. ЗАПУСКАЕМ АСИНХРОННУЮ ЗАДАЧУ 
        auto client_future = std::async(std::launch::async,
            [this](SOCKET socket, std::promise<std::string> promise) {
                this->handle_client(socket, std::move(promise));
            },
            client_socket,
            std::move(result_promise)
        );

        // 14. СОХРАНЯЕМ FUTURE ДЛЯ МОНИТОРИНГА 
        _client_futures.push_back(std::move(client_future));

        // 15. ПРОВЕРЯЕМ ЗАВЕРШЕННЫЕ ЗАДАЧИ И ВЫВОДИМ РЕЗУЛЬТАТЫ
     
        _client_futures.erase(
            std::remove_if(_client_futures.begin(), _client_futures.end(),
                [](std::future<void>& f) {
                    // Проверяем завершилась ли задача (неблокирующая проверка)
                    if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                        // Если нужно было получить строку-результат, мы бы сделали:
                        // std::string result = result_future.get();
                        // std::cout << "Результат обработки: " << result << std::endl;
                        return true;  // Удаляем завершенную future
                    }
                    return false;
                }),
            _client_futures.end()
        );

        std::cout << "Активных клиентских задач: " << _client_futures.size() << std::endl;
    }

    std::cout << "Сервер остановил прослушивание" << std::endl;
}

void TcpServer::stop_server()
{
    if (!_is_running) return;

    std::cout << "Остановка сервера..." << std::endl;
    _is_running = false;

    // Закрытие сокета прослушивания
    if (_listen_socket != INVALID_SOCKET) {
        closesocket(_listen_socket);
        _listen_socket = INVALID_SOCKET;
    }

    // 16. ОЖИДАЕМ ЗАВЕРШЕНИЯ ВСЕХ КЛИЕНТСКИХ ПОТОКОВ 
    std::cout << "Ожидание завершения " << _client_futures.size() << " клиентских задач..." << std::endl;

    for (auto& future : _client_futures) {
        if (future.valid()) {
 
            future.wait();
        }
    }
    _client_futures.clear();

    WSACleanup();
    std::cout << "Сервер полностью остановлен" << std::endl;
}