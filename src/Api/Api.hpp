#pragma once
#include "../AConfig/AConfig.hpp"
#include "../Request/Request.hpp"
#include "../Server/Server.hpp"
#include "../../includes.hpp"

class Server;
class Request;
class Api
{
    private:
        Request         *_request;
        Server          *_server;
        std::string     _httpResponse;
        std::string     _errorPath = "www/errors";
        int             _client_socket;
    public:
        Api();
        ~Api();
        Api(const Api& other);
        Api& operator=(const Api& other);

        void    setRequest(Request *request);
        void    setServer(Server *server);
        
        void    handleRequest(int);

        int     checkMethod();
        void    createResponse();
        void    sendError(int);

        void prepareRedirectResponse(const std::string& newLocation);
        void prepareHtmlResponse(const std::string& htmlContent);
        void prepareJsonResponse(const std::string& jsonContent);
        void sendResponse(int client_socket);

};


