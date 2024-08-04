#include "Api.hpp"


Api::Api() {}

Api::~Api() {}


void    Api::sendResponse(Request  request, Server *server, int client_socket)
{
    std::vector<std::string> serverMethods = server->getMethods();
    
    std::cout << "da segfault" << std::endl << std::endl;

    for (std::vector<std::string>::iterator it = serverMethods.begin(); it < serverMethods.end(); it++)
    {
        std::string method = *it;
        std::cout << method << std::endl;
    }
    std::cout << "no da segfault" << std::endl << std::endl;
    

    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 14\r\n"
        "\r\n"
        "Hello, Client!";

    send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);
}