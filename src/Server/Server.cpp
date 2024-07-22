#include "Server.hpp"

Server::Server()
{
}

Server::Server(const Server &src)
{
	*this = src;
}

Server &Server::operator=(const Server &rhs)
{
	if (this == &rhs)
		return (*this);
	return (*this);
}

Server::~Server()
{
}

