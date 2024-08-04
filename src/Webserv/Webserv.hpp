#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include "../AConfig/AConfig.hpp"
#include "../Api/Api.hpp"

class Webserv : public AConfig
{
	private:
		std::vector<int> 		_client_sockets;
		Api						_api;
		Request					_request;

	public:
		Webserv(std::string config_file);
		Webserv(const Webserv &webserv);
		Webserv &operator=(const Webserv &webserv);
		~Webserv();

		int				setUpServers();
		void			runServers();
		int				readRequest(int);
		Server*			findServer(int);
};

#endif