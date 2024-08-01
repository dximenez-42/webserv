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
	this->setUp();
	this->listen();
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

int		Server::getServerFd() const
{
	return (_server_fd);
}

struct sockaddr_in	Server::getServerAddress() const
{
	return (_address);
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


void Server::handleConnections() {
	// Verificar si hay conexiones entrantes
	if (FD_ISSET(_server_fd, &_read_fds)) {
		int new_socket = ::accept(_server_fd, NULL, NULL);
		if (new_socket < 0) {
			std::cerr << "Error en accept" << std::endl;
			exit(EXIT_FAILURE);
		}
		_client_sockets.push_back(new_socket);
		std::cout << "Nueva conexión aceptada" << std::endl;
	}

	// Verificar actividad en clientes existentes
	for (size_t i = 0; i < _client_sockets.size(); ++i) {
		if (FD_ISSET(_client_sockets[i], &_read_fds)) {
			char buffer[1024] = {0};
			int valread = ::read(_client_sockets[i], buffer, 1024);
			if (valread == 0) {
				// Cliente desconectado
				close(_client_sockets[i]);
				_client_sockets.erase(_client_sockets.begin() + i);
				--i; // Ajustar índice tras eliminar
				std::cout << "Cliente desconectado" << std::endl;
			} else {
				// Responder al cliente
				std::cout << "Mensaje recibido: " << buffer << std::endl;
				::send(_client_sockets[i], "Hello, Client!", 14, 0);
			}
		}
	}
}


int Server::setUp() 
{
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == 0) {
        std::cerr << "Socket failed" << std::endl;
        return -1;
    }

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_server_port);

    if (bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(_server_fd);
        return -1;
    }
	std::cout << "Server " << _server_name << " setted up" << std::endl;
	return 0;
}


int Server::listen() {
    if (::listen(_server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(_server_fd);
        return -1;
    }

    std::cout << "Server " << _server_name << " listening on port " << _server_port << std::endl;
	return 0;
}


int Server::accept() {
	int new_socket;
    int addrlen = sizeof(_address);
	std::cout << htons(_address.sin_port) << std::endl;
	
    new_socket = ::accept(_server_fd, (struct sockaddr*)&_address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(_server_fd);
		exit(EXIT_FAILURE);
    }
	else
		std::cout << "Connection in " << _server_name << " on port " << _server_port << " accepted" << std::endl;

	return new_socket;
}

int Server::read() {
	char buffer[BUFFER_SIZE] = {0};

	int valread = ::read(_server_socket, buffer, BUFFER_SIZE);
	if (valread > 0)
    	std::cout << "Received message: " << buffer << std::endl;
	else 
    	std::cout << "Empty message" << std::endl;
	return 0;
}

int Server::send() {
    const char* hello = "Hello from server";

	::send(_server_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent" << std::endl;
	return 0;
}
