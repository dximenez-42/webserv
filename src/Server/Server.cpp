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
	std::cout << "Server destruct" << std::endl;
}

int Server::setup() {
	int _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == 0) {
        std::cerr << "Socket creation error\n";
        return -1;
    }

	setAddress();

	if (bind(_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
        std::cerr << "Bind failed\n";
        close(_fd);
        return -1;
    }

	if (listen(_fd, 3) < 0) {
        std::cerr << "Listen failed\n";
        close(_fd);
        return -1;
    }

    std::cout << "Server listening on port " << _port << std::endl;
	return 0;
}

void Server::setAddress() {
	_address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
}

int Server::accept() {
	int new_socket;
	int addrlen = sizeof(_address);

	while (new_socket = ::accept(_fd, NULL, NULL) >= 0) {
		std::cout << "Connection accepted\n";
		char buffer[1024] = {0};
		read(new_socket, buffer, 1024);
		std::cout << "Request: \n" << buffer << "\n";
	}

	if (new_socket < 0) {
		std::cerr << "Accept failed\n";
		close(_fd);
		return -1;
	}
	return 0;
}
