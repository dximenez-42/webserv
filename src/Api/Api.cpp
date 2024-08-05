#include "Api.hpp"


Api::Api() {}

Api::~Api() {}


void    Api::setRequest(Request* request) {
    std::cout << "Request setted" << std::endl;
}

void    Api::sendResponse(Server *server, int client_socket)
{
    std::cout << "Servidor seleccionado: " << std::endl;
    std::cout << "Cliente socket: " << client_socket << std::endl;

    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 14\r\n"
        "\r\n"
        "Hello, Client!";

    send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);
}