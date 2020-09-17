#include "Client.hpp"

#include <iostream>
#include <thread>

using memo::Client;

int main(int argc, char** argv)
{
  	try
  	{
  	    if (argc != 3)
  	    {
  	      std::cerr << "Usage: http_server <address> <port>\n";
  	      std::cerr << "Ex   : http_server 127.0.0.1 8000\n";
  	      return 1;
        }
  	}
  	catch (std::exception& e)
  	{
        std::cout << "exception:\n" << e.what() << std::endl;
  	}

    const std::string ipAddress = argv[1];
    const std::string port = argv[2];
    Client client(ipAddress+":"+port);
    client.run();

    return 0;
}
