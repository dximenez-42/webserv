#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes.hpp"
#include "../Config/Config.hpp"
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>

class Server : public Config
{
	private:
        long                        _fd;
        struct sockaddr_in          _address;
        int                         _port;
        std::map<long, Server>		_servers;
        std::map<long, Server *>	_sockets;

	public:
		Server(std::string filename);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

		int accept();
        int setup();
        void setAddress();
};


#endif