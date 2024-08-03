#include "Api.hpp"

Api::Api(Server& server) : _server(server) {}

Api::~Api() {}


void   Api::setServer(Server *server)
{
    _server = server;
};


int     Api::isMethodAccepted()
{
    if (_request.getMethod() == _servers.getMethods())
};

int     Api::pathExists()
{

};

void    Api::sendResponse(int client_socket)
{
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 14\r\n"
        "\r\n"
        "Hello, Client!";

    send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);
}