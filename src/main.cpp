#include "Client.hpp"

#include <iostream>
#include <thread>

using memo::client::Client;

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

    // Spawn reader thread that loops indefinitely
    std::thread thread_ = std::thread(&Client::AsyncCompleteRpc, &client);

    client.memoSearch("Test title1");
    client.tagSearch("Test tagName");
    client.memoSearchById("Test-id-102934048320");
    client.createMemo("Test-memo-create");
    client.createTag("Test-tag-create");
    client.updateMemo("Test-memo-update");
    client.updateTag("Test-tag-update");
    client.deleteMemo("Test-memo-id-delete-102930143290");
    client.deleteTag("Test-tag-name-delete-");

    std::cin.ignore();

    client.stop();
    thread_.join();

    return 0;
}
