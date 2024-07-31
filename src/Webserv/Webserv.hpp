#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include "../AConfig/AConfig.hpp"

class Webserv : public AConfig
{
	private:

	public:
		Webserv(std::string config_file);
		Webserv(const Webserv &webserv);
		Webserv &operator=(const Webserv &webserv);
		~Webserv();
};

#endif