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
				ssize_t valread = readRequest(client_socket);
				if (valread == 0) {
					close(client_socket);
					std::cout << "Cliente desconectado" << std::endl;
					it = _client_sockets.erase(it);
				} else {
					_request.printRequest();
				//Aquí se gestionaría la API --------------------------------
					std::string httpResponse =
						"HTTP/1.1 200 OK\r\n"
						"Content-Type: text/plain\r\n"
						"Content-Length: 14\r\n"
						"\r\n"
						"Hello, Client!";

					send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);

				//-----------------------------------------------------------
					++it;
				}
			} else {
				++it;
			}
		}
	}
}

int		Webserv::readRequest(int client_socket) {
	std::vector<char> requestData;
	char buffer[BUFFER_SIZE];
	ssize_t valread;
	bool requestComplete = false;

	while (!requestComplete && (valread = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
		requestData.insert(requestData.end(), buffer, buffer + valread);

		std::string requestString(requestData.begin(), requestData.end());
		size_t headerEnd = requestString.find("\r\n\r\n");
		if (headerEnd != std::string::npos) {
			size_t contentLengthPos = requestString.find("Content-Length:");
			if (contentLengthPos != std::string::npos) {
				size_t contentLengthStart = contentLengthPos + 15;
				size_t contentLengthEnd = requestString.find("\r\n", contentLengthStart);
				std::string contentLengthStr = requestString.substr(contentLengthStart, contentLengthEnd - contentLengthStart);
				int contentLength = std::stoi(contentLengthStr);

				if (requestData.size() >= headerEnd + 4 + contentLength) {
					requestComplete = true;
				}
			} else if (requestString.find("Transfer-Encoding: chunked") != std::string::npos) {
				size_t pos = headerEnd + 4;
				while (pos < requestData.size()) {
					size_t chunkSizeEnd = requestString.find("\r\n", pos);
					int chunkSize = std::stoi(requestString.substr(pos, chunkSizeEnd - pos), nullptr, 16);
					pos = chunkSizeEnd + 2;
					pos += chunkSize + 2;
					if (chunkSize == 0) {
						requestComplete = true;
						break;
					}
				}
			} else {
				requestComplete = true;
			}
		}
	}
	std::string requestString(requestData.begin(), requestData.end());
	_request.fillRequest(requestString);
	return (valread);
}
