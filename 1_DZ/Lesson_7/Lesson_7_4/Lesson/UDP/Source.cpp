#include <iostream>
#include "UpdServer.h"


int main()
{

    setlocale(LC_ALL, "Russian");
    UdpServer server;
    server.recieve_and_answer();
    system("Pause");

    return 0;
}
