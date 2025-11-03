#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

class UdpServer
{
private:
	WSAData _wsa_data;        // объект для сокета, Хранит информацию о инициализации Winsock
	SOCKET _socket;           // Дескриптор сокета - идентификатор нашего "порта"
	sockaddr_in _server_addr{}; // Адрес сервера (структура)
	sockaddr_in client_addr{};  // Адрес клиента (структура) 
	char* _buffer = new char[32]; // Буфер для приема данных (32 байта)
	int _port = 12345;        // Порт для прослушивания
public:
	UdpServer()
	{
		// 1. Инициализация Winsock
		int err = WSAStartup(MAKEWORD(2, 2), &_wsa_data);     // MAKEWORD(2, 2) - запрашиваем версию Winsock 2.2
    // &_wsa_data - указатель, куда запишутся данные инициализации
		_socket = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET - IPV4, SOCK_DGRAM - UDP,  // 0 - протокол по умолчанию для указанного типа
		//Настройка адреса  
		_server_addr.sin_family = AF_INET; // Семейство адресов IPv4
		_server_addr.sin_addr.s_addr = INADDR_ANY; // Принимать соединения с любых интерфейсов //установка адреса- любой 
		_server_addr.sin_port = htons(_port); //htons преобразует число в сетевой порядок байт Порт в сетевом порядке байт
		// 4. Привязка сокета к адресу
		int err_s = bind(_socket, (sockaddr*)&_server_addr, sizeof(_server_addr));// подкл сокета  sockaddr - базовый указатель  // (sockaddr*)&_server_addr - приведение к базовому типу sockaddr*
		if (err_s == SOCKET_ERROR) { 
			std::cout << "Сокет не создан " << std::endl;
			closesocket(_socket);
			WSACleanup();
		}
	}

	~UdpServer()
	{
		closesocket(_socket);
		WSACleanup();
	}

	void recieve_and_answer() 
	{
		int client_size = sizeof(client_addr); // Размер структуры адреса клиента
		while (true) {  // Бесконечный цикл для постоянной работы сервера
			std::cout << "Ждем клиента " << std::endl;
			// Получение данных от клиента
			int size_data = recvfrom(_socket, _buffer, 32, 0, (sockaddr*)&client_addr, &client_size); //чтение, ждет данные, _buffer - где леж данные
			// _socket - сокет для приема
// _buffer - буфер для записи данных
// 32 - максимальный размер данных
// 0 - флаги (обычно 0)
// (sockaddr*)&client_addr - адрес клиента (заполнится после вызова)
// &client_size - указатель на размер структуры
			if (size_data > 0) {
				std::cout << _buffer << std::endl;
				std::string answer = " Сервер ... ";
				sendto(_socket, answer.c_str(), 32, 0, (sockaddr*)&client_addr, client_size);
			}
		}


	}
};