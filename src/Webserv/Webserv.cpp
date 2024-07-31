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
