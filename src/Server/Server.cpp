#include "Server.hpp"

Server::Server()
{
	_server_port = 0;
	_server_host = "";
	_server_name = "default_name";
	_server_index = "";
	_server_dir_listing = -1;
	_server_root = "";
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

int				Server::getLimitBodySize() const
{
	return (_limit_body_size);
}

std::string		Server::getErrorsDir() const
{
	return (_errors_dir);
}

std::vector<std::string>	Server::getMethods() const
{
	return (_methods);
}

std::vector<Route>			Server::getRoutes() const
{
	return (_routes);
}

int		Server::getServerFd() const
{
	return (_server_fd);
}

struct sockaddr_in	Server::getServerAddress() const
{
	return (_address);
}

std::vector<int> 	Server::getClientSockets() const
{
	return _client_sockets;
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

void	Server::setLimitBodySize(int limit_body_size)
{
	_limit_body_size = limit_body_size;
}

void	Server::setErrorsDir(std::string dir)
{
	_errors_dir = dir;
}

void	Server::addMethod(std::string method)
{
	_methods.push_back(method);
}

void	Server::addRoute(Route route)
{
	_routes.push_back(route);
}

void Server::pushClientSocket(int clientSocket) {
	_client_sockets.push_back(clientSocket);
}

bool Server::hasClientSocket(int client_socket) const {
    return std::find(_client_sockets.begin(), _client_sockets.end(), client_socket) != _client_sockets.end();
}


int Server::setUp() 
{
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == 0) {
        return -1;
    }

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = inet_addr(_server_host.c_str());
    _address.sin_port = htons(_server_port);

    if (bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
        close(_server_fd);
		std::cout << "Server couldn't listen on that ip and/or port" << std::endl;
        return -1;
    }
	return 0;
}


int Server::listen() {
    if (::listen(_server_fd, 3) < 0) {
        close(_server_fd);
        return -1;
    }
	return 0;
}


int Server::accept() {
	int new_socket;
    int addrlen = sizeof(_address);
	
    new_socket = ::accept(_server_fd, (struct sockaddr*)&_address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        close(_server_fd);
		exit(EXIT_FAILURE);
    }

	return new_socket;
}

int Server::read() {
	char buffer[BUFFER_SIZE] = {0};

	::read(_server_socket, buffer, BUFFER_SIZE);
	return 0;
}

int Server::send() {
    const char* hello = "Hello from server";

	::send(_server_socket, hello, strlen(hello), 0);
	return 0;
}
