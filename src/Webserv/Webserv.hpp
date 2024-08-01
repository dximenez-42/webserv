#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include "../AConfig/AConfig.hpp"

class Webserv : public AConfig
{
	private:
		std::vector<int> 	_client_sockets;
		std::vector<Server*>	_servers;
	
	public:
		Webserv(std::string config_file);
		Webserv(const Webserv &webserv);
		Webserv &operator=(const Webserv &webserv);
		~Webserv();

		std::vector<Server*>	getServers() const;

		void	printServers();

		void	runServers();
		int		setUpServers();


};

#endif