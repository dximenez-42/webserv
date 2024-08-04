#pragma once
#include "../AConfig/AConfig.hpp"
#include "../Request/Request.hpp"
#include "../Server/Server.hpp"

class Server;
class Request;
class Api
{
    private:

    public:
        Api();
        ~Api();

        void    sendResponse(Request , Server*, int client_socket);
};


