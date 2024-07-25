#include "Server.hpp"

Server::Server()
{
	_server_port = 0;
	_server_host = "";
	_server_name = "default_name";
	_server_index = "";
	_server_dir_listing = -1;
	_server_root = "";
	_server_public = "";
	_limit_body_size = 0;
}

Server::Server(const Server &src)
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

int				Server::getServerPort() const
{
	return (_server_port);
}

std::string		Server::getServerHost() const
{
	return (_server_host);
}

std::string		Server::getServerName() const
{
	return (_server_name);
}

std::string		Server::getServerIndex() const
{
	return (_server_index);
}

int			Server::getServerDirListing() const
{
	return (_server_dir_listing);
}

std::string		Server::getServerRoot() const
{
	return (_server_root);
}

std::string		Server::getServerPublic() const
{
	return (_server_public);
}

int				Server::getLimitBodySize() const
{
	return (_limit_body_size);
}

std::vector<ErrorPage>		Server::getErrorPages() const
{
	return (_error_pages);
}

std::vector<std::string>	Server::getMethods() const
{
	return (_methods);
}

std::vector<Route>			Server::getRoutes() const
{
	return (_routes);
}

void	Server::setServerPort(int port)
{
	_server_port = port;
}

void	Server::setServerHost(std::string host)
{
	_server_host = host;
}

void	Server::setServerName(std::string name)
{
	_server_name = name;
}

void	Server::setServerIndex(std::string index)
{
	_server_index = index;
}

void	Server::setServerDirListing(int dir_listing)
{
	_server_dir_listing = dir_listing;
}

void	Server::setServerRoot(std::string root)
{
	_server_root = root;
}

void	Server::setServerPublic(std::string public_dir)
{
	_server_public = public_dir;
}

void	Server::setLimitBodySize(int limit_body_size)
{
	_limit_body_size = limit_body_size;
}

void	Server::addErrorPage(ErrorPage error)
{
	_error_pages.push_back(error);
}

void	Server::addMethod(std::string method)
{
	_methods.push_back(method);
}

void	Server::addRoute(Route route)
{
	_routes.push_back(route);
}

