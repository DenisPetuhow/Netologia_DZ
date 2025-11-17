#include <iostream>
#include "UpdClients.h"


int main()
{

    setlocale(LC_ALL, "Russian");
    UdpClients client;
    client.send_text();

    return 0;
}
