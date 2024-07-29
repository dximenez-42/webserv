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
	std::cout << "Server destruct" << std::endl;
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


int Server::setUp() {
    struct sockaddr_in address;

    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == 0) {
        std::cerr << "Socket failed" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_server_port);

    if (bind(_server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(_server_fd);
        return -1;
    }

	return 0;
}


int Server::listen() {
    if (::listen(_server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(_server_fd);
        return -1;
    }

    std::cout << "Server listening on port " << _server_port << std::endl;
	
	return 0;
}


int Server::accept() {
    int addrlen = sizeof(address);

    _server_socket = ::accept(_server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (_server_socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(_server_fd);
        return -1;
    }

	return 0;
}

int Server::read() {
	char buffer[BUFFER_SIZE] = {0};

	int valread = ::read(_server_socket, buffer, BUFFER_SIZE);
    std::cout << "Received message: " << buffer << std::endl;
	return 0;
}

int Server::send() {
    const char* hello = "Hello from server";

	::send(_server_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent" << std::endl;
	return 0;
}
