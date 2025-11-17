#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

class UdpClients
{
private:
	WSAData _wsa_data_c;  //объект, данные для сокета
	SOCKET _socket;
	sockaddr_in _server_addr{}, client_addr{}; // объявление структуры
	char* _buffer = new char[32];
	int _port = 12345;
public:
	UdpClients() // СОЗДАЛ сокет и кинул данные
	{
		int err = WSAStartup(MAKEWORD(2, 2), &_wsa_data_c); //Запускаем сокет
		_socket = socket(AF_INET, SOCK_DGRAM, 0); // AF_INET - IPV4, SOCK_DGRAM - UDP
		//Настройка адреса  
		_server_addr.sin_family = AF_INET;
		std::string ip = "127.0.0.1"; // localhost
		_server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); //установка адреса- любой 
		_server_addr.sin_port = htons(_port); //htons чтобы перевернуть последов бит
	}

	~UdpClients()
	{
		closesocket(_socket);
		WSACleanup();

	}

	void send_text()
	{
		int server_size = sizeof(_server_addr);
		std::string hello = " Клиент ... ";
		// Отправка данных серверу
		sendto(_socket, hello.c_str(), 32, 0, (sockaddr*)&_server_addr, server_size);//данные передал
		// Ожидание ответа от сервера
		int size_data = recvfrom(_socket, _buffer, 32, 0, (sockaddr*)&_server_addr, &server_size); //чтение, ждет данные, _buffer - где леж данные
		if (size_data > 0) {
			std::cout << _buffer << std::endl;
			std::cout << "end  " << std::endl;
		}
	}

};
