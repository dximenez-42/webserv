#include "Server.hpp"

Server::Server(std::string filename) : Config(filename)
{
}

Server::Server(const Server &src) : Config(src)
{
	*this = src;
}

Server &Server::operator=(const Server &rhs)
{
	if (this == &rhs)
	{
		return (*this);
	}
	return (*this);
}

Server::~Server()
{
	if (!_error_pages.empty())
		_error_pages.clear();
	if (!_methods.empty())
		_methods.clear();
	if (!_routes.empty())
		_routes.clear();
}
