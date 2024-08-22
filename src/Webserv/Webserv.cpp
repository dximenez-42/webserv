#include "Webserv.hpp"

Webserv::Webserv(std::string config_file) : AConfig(config_file)
{
	this->printServers();
	this->setUpServers();
	this->runServers();
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

Webserv::~Webserv() {}

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

		for (std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
    		Server* server = *it;
			FD_SET(server->getServerFd(), &readfds);
			max_sd = std::max(max_sd, server->getServerFd());
		}

		for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end(); ++it) {
    		int client_socket = *it;
			FD_SET(client_socket, &readfds);
			max_sd = std::max(max_sd, client_socket);
		}

		int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR)) {
			std::cerr << "Error en select" << std::endl;
			exit(EXIT_FAILURE);
		}

		for (std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
		    Server* server = *it;
			if (FD_ISSET(server->getServerFd(), &readfds)) {
				int new_socket = server->accept();
				_client_sockets.push_back(new_socket);
				_api.setServer(server);

				std::cout << "Nueva conexión aceptada en puerto: " << server->getServerPort() << std::endl;
			}
		}

		for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end();) {
    		int client_socket = *it;
			if (FD_ISSET(client_socket, &readfds)) {
				// char buffer[BUFFER_SIZE] = {0};		// TODO remove
				ssize_t valread = readRequest(client_socket);

				if (valread <= 0) {
					close(client_socket);
					std::cout << "Cliente desconectado" << std::endl;
					it = _client_sockets.erase(it);
				} else {
					_api.handleRequest(client_socket);
					delete _request;
					++it;
				}
			} else {
				++it;
			}
		}
	}
}

Server* Webserv::findServer(int client_socket) {
    for (std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
        Server* server = *it;
        if (server->hasClientSocket(client_socket)) {
            return server;
        }
    }
    return 0;
}


int Webserv::readRequest(int client_socket) {
    std::cout << std::endl << "Se lee la request" << std::endl;

    std::vector<char> requestData;
    char buffer[BUFFER_SIZE];
    ssize_t valread = 0;
    bool headersComplete = false;
    size_t contentLength = 0;


    while (!headersComplete && (valread = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        
		requestData.insert(requestData.end(), buffer, buffer + valread);
        std::string requestString(requestData.begin(), requestData.end());
        size_t headersEnd = requestString.find("\r\n\r\n");
        if (headersEnd != std::string::npos) {
            headersComplete = true;
            headersEnd += 4;

            size_t contentLengthPos = requestString.find("Content-Length: ");
            if (contentLengthPos != std::string::npos) {
                contentLengthPos += 16;
                size_t contentLengthEnd = requestString.find("\r\n", contentLengthPos);
                contentLength = stoi(requestString.substr(contentLengthPos, contentLengthEnd - contentLengthPos));
            }

            size_t bodySize = requestString.size() - headersEnd;
            while (bodySize < contentLength) {
                valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (valread > 0) {
                    requestData.insert(requestData.end(), buffer, buffer + valread);
                    bodySize += valread;
                } else {
                    break;
                }
            }
        }
    }

    std::string requestString(requestData.begin(), requestData.end());
    _request = new Request();
    _request->fillRequest(requestString);

    _api.setRequest(_request);

    return (valread);
}
