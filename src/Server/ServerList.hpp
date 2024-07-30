#pragma once 


#include "Server.hpp"

class ServerList
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
