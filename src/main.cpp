#include "Client.hpp"

#include <iostream>
#include <thread>
#include <locale>

using memo::Client;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: http_server <address> <port>\n";
        std::cerr << "Ex   : http_server 127.0.0.1 8000\n";
        return 1;
    }
    setlocale(LC_ALL, "");

    const std::string ipAddress = argv[1];
    const std::string port = argv[2];
    Client client(ipAddress+":"+port);
    client.run();
    //client.runcurses();

    return 0;
}
