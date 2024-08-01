#include "Webserv.hpp"

Webserv::Webserv(std::string config_file) : AConfig(config_file)
{
	
}

Webserv::Webserv(const Webserv &webserv) : AConfig(webserv)
{
	*this = webserv;
}

Webserv &Webserv::operator=(const Webserv &webserv)
{
	if (this != &webserv)
	{
		_servers = webserv._servers;
	}
	return *this;
}

Webserv::~Webserv()
{
}


void	Webserv::printServers()
{
	std::cout << "access_log:\t\t" << _access_log << std::endl;
	std::cout << "error_log:\t\t" << _error_log << std::endl << std::endl;

	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "\033[1;32mServer #" << i + 1 << "\033[0m" << std::endl;
		std::cout << "server_port:\t\t" << _servers[i]->getServerPort() << std::endl;
		std::cout << "server_name:\t\t" << _servers[i]->getServerName() << std::endl;
		std::cout << "server_index:\t\t" << _servers[i]->getServerIndex() << std::endl;
		std::cout << "server_dir_listing:\t" << _servers[i]->getServerDirListing() << std::endl;
		std::cout << "server_root:\t\t" << _servers[i]->getServerRoot() << std::endl;
		std::cout << "server_public:\t\t" << _servers[i]->getServerPublic() << std::endl;
		std::cout << "limit_body_size:\t" << _servers[i]->getLimitBodySize() << std::endl;

		if (!_servers[i]->getErrorPages().empty())
		{
			std::cout << "error_pages:" << std::endl;
			for (size_t j = 0; j < _servers[i]->getErrorPages().size(); j++)
			{
				std::cout	<< "\terror_code: \"" << _servers[i]->getErrorPages()[j].code
							<< "\", error_page: \"" << _servers[i]->getErrorPages()[j].path << "\"" << std::endl;
			}
		}
		if (!_servers[i]->getMethods().empty())
		{
			std::cout << "methods:" << std::endl;
			for (size_t j = 0; j < _servers[i]->getMethods().size(); j++)
			{
				std::cout << "\t" << _servers[i]->getMethods()[j] << std::endl;
			}
		}
		if (!_servers[i]->getRoutes().empty())
		{
			std::cout << "methods:" << std::endl;
			for (size_t j = 0; j < _servers[i]->getRoutes().size(); j++)
			{
				std::cout	<< "\tmethod: \"" << _servers[i]->getRoutes()[j].method
							<< "\", path: \"" << _servers[i]->getRoutes()[j].path
							<< "\", location: \"" << _servers[i]->getRoutes()[j].location << "\"" << std::endl;
			}
		}
		std::cout << std::endl << std::endl;
	}
	
}

int	Webserv::setUpServers()
{
	for (std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
		Server* server = *it;
		if(server->setUp() == -1)
			return -1;
		server->listen();
	}
	return 0;
}

void Webserv::runServers()
{
	fd_set readfds;
	int max_sd;

	while (true) {
		FD_ZERO(&readfds);
		max_sd = 0;

		// Agregar descriptores de los servidores
		for (std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
    		Server* server = *it;
			FD_SET(server->getServerFd(), &readfds);
			max_sd = std::max(max_sd, server->getServerFd());
		}

		// Agregar descriptores de los clientes
		for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end(); ++it) {
    		int client_socket = *it;
			FD_SET(client_socket, &readfds);
			max_sd = std::max(max_sd, client_socket);
		}

		// Esperar actividad
		int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR)) {
			std::cerr << "Error en select" << std::endl;
			exit(EXIT_FAILURE);
		}

		// Verificar nuevos clientes
		for (std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
		    Server* server = *it;
			if (FD_ISSET(server->getServerFd(), &readfds)) {
				int new_socket = server->accept();
				_client_sockets.push_back(new_socket);
				std::cout << "Nueva conexión aceptada en puerto: " << server->getServerPort() << std::endl;
			}
		}

		// Verificar actividad en los clientes
		for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end();) {
    		int client_socket = *it;
			if (FD_ISSET(client_socket, &readfds)) {
				char buffer[BUFFER_SIZE] = {0};
				ssize_t valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
				if (valread == 0) {
					close(client_socket);
					std::cout << "Cliente desconectado" << std::endl;
					it = _client_sockets.erase(it);
				} else {
					std::cout << "Mensaje recibido: " << buffer << std::endl;
					send(client_socket, "Hello, Client!", 14, 0);
					++it;
				}
			} else {
				++it;
			}
		}
	}
}
