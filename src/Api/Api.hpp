#pragma once
#include "../AConfig/AConfig.hpp"
#include "../Request/Request.hpp"
#include "../Server/Server.hpp"

class Server;
class Request;
class Api
{
    private:
        Request *_request;
    public:
        Api();
        ~Api();

        void    setRequest(Request *request);
        void    sendResponse(Server*, int client_socket);
};


