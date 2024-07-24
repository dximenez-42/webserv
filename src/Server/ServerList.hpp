#pragma once 

#include "../Config/Config.hpp"

#include "Server.hpp"

class ServerList : public Config
{
    private:
        fd_set                      _fd_set;
        long                        _max_fd;
        std::vector<int>	        _ready;
        std::map<long, Server>		_servers;
        std::map<long, Server *>	_sockets;

    public:
        ServerList();
        ~ServerList();

        void config(std::string);
        int setup();
        void run();
};
