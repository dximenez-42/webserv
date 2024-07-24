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


int			Server::send(long socket)
{
	static std::map<long, size_t>	sent;

	if (sent.find(socket) == sent.end())
		sent[socket] = 0;

	if (OUT && sent[socket] == 0)
	{
		if (_requests[socket].size() < 1000)
			std::cout << "\rResponse :                " << std::endl << "[" << GREEN << _requests[socket] << RESET << "]\n" << std::endl;
		else
			std::cout << "\rResponse :                " << std::endl << "[" << GREEN << _requests[socket].substr(0, 1000) << "..." << _requests[socket].substr(_requests[socket].size() - 10, 15) << RESET << "]\n" << std::endl;
	}

	std::string	str = _requests[socket].substr(sent[socket], RECV_SIZE);
	int	ret = ::send(socket, str.c_str(), str.size(), 0);

	if (ret == -1)
	{
		this->close(socket);
		sent[socket] = 0;
		return (-1);
	}
	else
	{
		sent[socket] += ret;
		if (sent[socket] >= _requests[socket].size())
		{
			_requests.erase(socket);
			sent[socket] = 0;
			return (0);
		}
		else
			return (1);
	}
}

//Recibe y lee la request
int			Server::recv(long socket)
{
	char	buffer[RECV_SIZE] = {0};
	int		ret;

	ret = ::recv(socket, buffer, RECV_SIZE - 1, 0);

	if (ret == 0 || ret == -1)
	{
		close(socket);
		if (!ret)
			std::cout << "\rConnection was closed by client.\n" << std::endl;
		else
			std::cout << "\rRead error, closing connection.\n" << std::endl;
		return (-1);
	}

	_requests[socket] += std::string(buffer);

	size_t	i = _requests[socket].find("\r\n\r\n");
	if (i != std::string::npos)
	{
		if (_requests[socket].find("Content-Length: ") == std::string::npos)
		{
			if (_requests[socket].find("Transfer-Encoding: chunked") != std::string::npos)
			{
				if (checkEnd(_requests[socket], "0\r\n\r\n") == 0)
					return (0);
				else
					return (1);
			}
			else
				return (0);
		}

		size_t	len = std::atoi(_requests[socket].substr(_requests[socket].find("Content-Length: ") + 16, 10).c_str());
		if (_requests[socket].size() >= len + i + 4)
			return (0);
		else
			return (1);
	}

	return (1);
}


long 	Server::getFd() {
	return _fd;
}


void	Server::close(int socket)
{
	if (socket > 0)
		::close(socket);
	_requests.erase(socket);
}


void	Server::clean(void)
{
	if (_fd > 0)
		::close(_fd);
	_fd = -1;
}