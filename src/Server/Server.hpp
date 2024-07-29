#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes.hpp"
#include <iostream>

struct ErrorPage
{
	std::string		code;
	std::string		path;
};

struct Route
{
	std::string		method;
	std::string		path;
	std::string		location;
};

class Server
{
	private:
		int				_server_port;
		std::string		_server_host;
		std::string		_server_name;
		std::string		_server_index;
		int				_server_dir_listing;
		std::string		_server_root;
		std::string		_server_public;
		int				_limit_body_size;
		
		std::vector<ErrorPage>		_error_pages;
		std::vector<std::string>	_methods;
		std::vector<Route>			_routes;

		int					_server_fd;
		int					_server_socket;
	    struct sockaddr_in 	address;

	public:
		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

		int				getServerPort() const;
		std::string		getServerHost() const;
		std::string		getServerName() const;
		std::string		getServerIndex() const;
		int				getServerDirListing() const;
		std::string		getServerRoot() const;
		std::string		getServerPublic() const;
		int				getLimitBodySize() const;
		std::vector<ErrorPage>		getErrorPages() const;
		std::vector<std::string>	getMethods() const;
		std::vector<Route>			getRoutes() const;

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

		int		setUp();
		int		listen();
		int		accept();
		int		read();
		int		send();
};


#endif