#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes.hpp"
#include <iostream>
#include "../Api/Api.hpp"

class Server
{
	private:
		int							_server_port;
		std::string					_server_host;
		std::string					_server_name;
		std::string					_server_index;
		int							_server_dir_listing;
		std::string					_server_root;
		std::string					_server_public;
		int							_limit_body_size;
		
		std::vector<ErrorPage>		_error_pages;
		std::vector<std::string>	_methods;
		std::vector<Route>			_routes;

		int							_server_fd;
		int							_server_socket;
	    struct sockaddr_in 			_address;
		fd_set 						_read_fds;
		std::vector<int> 			_client_sockets;

	public:
		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

		int							getServerPort() const;
		std::string					getServerHost() const;
		std::string					getServerName() const;
		std::string					getServerIndex() const;
		int							getServerDirListing() const;
		std::string					getServerRoot() const;
		std::string					getServerPublic() const;
		int							getLimitBodySize() const;
		std::vector<ErrorPage>		getErrorPages() const;
		std::vector<std::string>	getMethods() const;
		std::vector<Route>			getRoutes() const;
		int							getServerFd() const;
		struct sockaddr_in			getServerAddress() const;
		std::vector<int> 			getClientSockets() const;

		void	setServerPort(int port);
		void	setServerHost(std::string host);
		void	setServerName(std::string name);
		void	setServerIndex(std::string index);
		void	setServerDirListing(int dir_listing);
		void	setServerRoot(std::string root);
		void	setServerPublic(std::string public_dir);
		void	setLimitBodySize(int limit_body_size);
		void	addErrorPage(ErrorPage error);
		void	addMethod(std::string method);
		void	addRoute(Route route);

		void	handleConnections();
		int		setUp();
		int		listen();
		int		accept();
		int		read();
		int		send();
};


#endif