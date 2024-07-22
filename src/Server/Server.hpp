#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes.hpp"
#include "../Config/Config.hpp"

class Server : public Config
{
	public:
		Server(std::string filename);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();
};


#endif