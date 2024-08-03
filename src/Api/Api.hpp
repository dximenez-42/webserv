#pragma once
#include "../AConfig/AConfig.hpp"

class Api : public AConfig
{
    private:
        Server& _server;

    public:
        Api(Server& server);
        ~Api();

        void    setServer(Server *server);
        int     isMethodAccepted();
        int     pathExists();
        void    sendResponse(int);
};


