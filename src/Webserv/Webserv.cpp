#include "Webserv.hpp"

Webserv::Webserv(std::string config_file)
{
	_configFile = config_file;
	_input.open(config_file.c_str(), std::ifstream::in);
	if (!_input.good() || !_input.is_open())
		throw FileOpenException();

	_access_log = "";
	_error_log = "";
	parseConfig();
}

Webserv::Webserv(const Webserv &webserv)
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
	if (_input.is_open())
		_input.close();
	if (!_servers.empty())
	{
		for (size_t i = 0; i < _servers.size(); i++)
		{
			delete _servers[i];
		}
		_servers.clear();
	}
	if (!_parse_errors.empty())
		_parse_errors.clear();
	if (!_open_blocks.empty())
		_open_blocks.clear();
}



void	Webserv::parseHttp(std::vector<std::string>& split, unsigned int line_number)
{
	if (split.size() != 2)
		return newError(line_number, "invalid line in http block");
	if (_open_blocks.size() != 1)
	{
		_open_blocks.push_back(BAD);
		return newError(line_number, "invalid http block nesting");
	}

	if (split[0] == "access_log" && _access_log.empty())
		_access_log = split[1];
	else if (split[0] == "access_log" && !_access_log.empty())
		return newError(line_number, "access_log already defined");
	else if (split[0] == "error_log" && _error_log.empty())
		_error_log = split[1];
	else if (split[0] == "error_log" && !_error_log.empty())
		return newError(line_number, "error_log already defined");
	else
		return newError(line_number, "invalid field \"" + split[0] + "\" in http block");
}

void	Webserv::parseServer(std::vector<std::string>& split, unsigned int line_number)
{
	Server	*server = _servers.back();

	if (split.size() != 2)
		return newError(line_number, "invalid line in server block");
	if (_open_blocks.size() != 2 || _open_blocks.at(0) != HTTP || _open_blocks.at(1) != SERVER)
	{
		_open_blocks.push_back(BAD);
		return newError(line_number, "invalid server block nesting");
	}

	if (split[0] == "port" && server->getServerPort() == 0)
	{
		server->setServerPort(std::atoi(split[1].c_str()));
		if (server->getServerPort() <= 0)
			return newError(line_number, "port must be greater than 0");
	}
	else if (split[0] == "port" && server->getServerPort() != 0)
		return newError(line_number, "port already defined");
	else if (split[0] == "host" && server->getServerHost().empty())
	{
		server->setServerHost(split[1]);
	}
	else if (split[0] == "host" && !server->getServerHost().empty())
		return newError(line_number, "host already defined");
	else if (split[0] == "name" && server->getServerName() == "default_name")
	{
		server->setServerName(split[1]);
	}
	else if (split[0] == "name" && server->getServerName() != "default_name")
		return newError(line_number, "name already defined");
	else if (split[0] == "index" && server->getServerIndex().empty())
	{
		server->setServerIndex(split[1]);
	}
	else if (split[0] == "index" && !server->getServerIndex().empty())
		return newError(line_number, "index already defined");
	else if (split[0] == "dir_listing" && server->getServerDirListing() == -1)
	{
		if (split[1] == "true")
			server->setServerDirListing(1);
		else if (split[1] == "false")
			server->setServerDirListing(0);
	}
	else if (split[0] == "dir_listing" && server->getServerDirListing() != -1)
		return newError(line_number, "dir_listing already defined");
	else if (split[0] == "root" && server->getServerRoot().empty())
	{
		server->setServerRoot(split[1]);
	}
	else if (split[0] == "root" && !server->getServerRoot().empty())
		return newError(line_number, "root already defined");
	else if (split[0] == "public" && server->getServerPublic().empty())
	{
		server->setServerPublic(split[1]);
	}
	else if (split[0] == "public" && !server->getServerPublic().empty())
		return newError(line_number, "public already defined");
	else if (split[0] == "limit_body_size" && server->getLimitBodySize() == 0)
	{
		server->setLimitBodySize(std::atoi(split[1].c_str()));
		if (server->getLimitBodySize() <= 0)
			return newError(line_number, "limit_body_size must be greater than 0");
	}
	else if (split[0] == "limit_body_size" && server->getLimitBodySize() != 0)
		return newError(line_number, "limit_body_size already defined");
	else
		return newError(line_number, "invalid field \"" + split[0] + "\" in server block");
}

void	Webserv::parseErrors(std::vector<std::string>& split, unsigned int line_number)
{
	if (split.size() != 2)
		return newError(line_number, "invalid line in errors block");
	if (_open_blocks.size() != 3 || _open_blocks.at(0) != HTTP || _open_blocks.at(1) != SERVER || _open_blocks.at(2) != ERRORS)
	{
		_open_blocks.push_back(BAD);
		return newError(line_number, "invalid location of errors block");
	}

	ErrorPage	error;
	error.code = split[0];
	error.path = split[1];

	if (checkErrorExists(error))
		return newError(line_number, "error code " + error.code + " already defined");
	_servers.back()->addErrorPage(error);
}

void	Webserv::parseMethods(std::vector<std::string>& split, unsigned int line_number)
{
	if (split.size() != 1)
		return newError(line_number, "invalid line in methods block");
	if (_open_blocks.size() != 3 || _open_blocks.at(0) != HTTP || _open_blocks.at(1) != SERVER || _open_blocks.at(2) != METHODS)
	{
		_open_blocks.push_back(BAD);
		return newError(line_number, "invalid location of methods block");
	}

	if (split[0] != "GET" && split[0] != "POST" && split[0] != "DELETE")
		return newError(line_number, "method " + split[0] + " is unsupported");

	if (checkMethodExists(split[0]))
		return newError(line_number, "method " + split[0] + " already defined");
	_servers.back()->addMethod(split[0]);
}

void	Webserv::parseRoutes(std::vector<std::string>& split, unsigned int line_number)
{
	if (split.size() != 3)
		return newError(line_number, "invalid line in routes block");
	if (_open_blocks.size() != 3 || _open_blocks.at(0) != HTTP || _open_blocks.at(1) != SERVER || _open_blocks.at(2) != ROUTES)
	{
		_open_blocks.push_back(BAD);
		return newError(line_number, "invalid location of routes block");
	}

	Route	route;
	route.method = split[0];
	route.path = split[1];
	route.location = split[2];
	if (split[0] != "GET" && split[0] != "POST" && split[0] != "DELETE")
		return newError(line_number, "method " + split[0] + " is unsupported");
	if (checkRouteExists(route))
		return newError(line_number, "route \"" + route.path + "\" with method " + route.method + " already defined");
	_servers.back()->addRoute(route);
}

void	Webserv::parseLine(std::string line, unsigned int line_number)
{
	std::vector<std::string>	split = ::splitSpaces(line);
	static bool	http_block = false;
	static bool	server_block = false;
	static bool	errors_block = false;
	static bool	methods_block = false;
	static bool	routes_block = false;

	if (split.size() >= 1 && split[0][0] == '#')
		return ;
	if (split.size() == 2 && split[1] == "{")
	{
		if (split[0] == "http" && !http_block)
		{
			http_block = true;
			_open_blocks.push_back(HTTP);
			_http_location = line_number;
		}
		else if (split[0] == "http" && http_block)
		{
			_open_blocks.push_back(HTTP);
			_open_blocks.push_back(BAD);
			return newError(line_number, "http block already defined");
		}
		else if (split[0] == "server" && !server_block)
		{
			server_block = true;
			_open_blocks.push_back(SERVER);
			_server_location = line_number;
			_servers.push_back(new Server());
		}
		else if (split[0] == "server" && server_block)
		{
			_open_blocks.push_back(SERVER);
			_open_blocks.push_back(BAD);
			return newError(line_number, "server block already defined");
		}
		else if (split[0] == "errors" && !errors_block)
		{
			errors_block = true;
			_open_blocks.push_back(ERRORS);
			_errors_location = line_number;
		}
		else if (split[0] == "errors" && errors_block)
		{
			_open_blocks.push_back(ERRORS);
			_open_blocks.push_back(BAD);
			return newError(line_number, "errors block already defined");
		}
		else if (split[0] == "methods" && !methods_block)
		{
			methods_block = true;
			_open_blocks.push_back(METHODS);
			_methods_location = line_number;
		}
		else if (split[0] == "methods" && methods_block)
		{
			_open_blocks.push_back(METHODS);
			_open_blocks.push_back(BAD);
			return newError(line_number, "methods block already defined");
		}
		else if (split[0] == "routes" && !routes_block)
		{
			routes_block = true;
			_open_blocks.push_back(ROUTES);
			_routes_location = line_number;
		}
		else if (split[0] == "routes" && routes_block)
		{
			_open_blocks.push_back(ROUTES);
			_open_blocks.push_back(BAD);
			return newError(line_number, "routes block already defined");
		}
		else
			return newError(line_number, "invalid block \"" + split[0] + "\"");
	}
	else if (split.size() == 1 && split[0] == "}")
	{
		if (_open_blocks.empty())
			return newError(line_number, "closing brackets without opening block");
		if (_open_blocks.back() == BAD)
			_open_blocks.pop_back();
		switch (_open_blocks.back())
		{
		case SERVER:
			server_block = false;
			break;
		case ERRORS:
			errors_block = false;
			break;
		case METHODS:
			methods_block = false;
			break;
		case ROUTES:
			routes_block = false;
			break;
		}
		_open_blocks.pop_back();
	}
	else
	{
		if (_open_blocks.empty())
			return newError(line_number, "invalid field \"" + split[0] + "\"");
		if (_open_blocks.back() == HTTP)
			parseHttp(split, line_number);
		else if (_open_blocks.back() == SERVER)
			parseServer(split, line_number);
		else if (_open_blocks.back() == ERRORS)
			parseErrors(split, line_number);
		else if (_open_blocks.back() == METHODS)
			parseMethods(split, line_number);
		else if (_open_blocks.back() == ROUTES)
			parseRoutes(split, line_number);
		else if (_open_blocks.back() != BAD)
			return newError(line_number, "invalid field \"" + split[0] + "\"");
	}
}

void	Webserv::parseConfig()
{
	unsigned int				line_number = 0;
	std::string					line;

	while (std::getline(_input, line))
	{
		++line_number;
		if (line.empty() || ::onlySpaces(line))
			continue;
		parseLine(line, line_number);
	}
	checkConfig();
	if (_input.is_open())
		_input.close();
	if (!_parse_errors.empty())
		_parse_errors.clear();
	if (!_open_blocks.empty())
		_open_blocks.clear();
}

void	Webserv::checkConfig()
{
	std::vector<std::string>	errors;

	if (_access_log.empty())
		errors.push_back("access_log");
	if (_error_log.empty())
		errors.push_back("error_log");

	for (size_t i = 0; i < _servers.size(); i++)
	{
		if (_servers[i]->getServerPort() == 0)
			errors.push_back("port");
		if (_servers[i]->getServerHost().empty())
			errors.push_back("host");
		if (_servers[i]->getServerIndex().empty())
			errors.push_back("index");
		if (_servers[i]->getServerDirListing() == -1)
			errors.push_back("dir_listing");
		if (_servers[i]->getServerRoot().empty())
			errors.push_back("root");
		if (_servers[i]->getServerPublic().empty())
			errors.push_back("public");
		if (_servers[i]->getLimitBodySize() == 0)
			errors.push_back("limit_body_size");
		if (_servers[i]->getMethods().empty())
			errors.push_back("methods");
	}

	if (_open_blocks.size() != 0)
	{
		for (size_t i = 0; i < _open_blocks.size(); i++)
		{
			if (_open_blocks[i] == HTTP)
				newError(_http_location, "http block not closed");
			else if (_open_blocks[i] == SERVER)
				newError(_server_location, "server block not closed");
			else if (_open_blocks[i] == ERRORS)
				newError(_errors_location, "errors block not closed");
			else if (_open_blocks[i] == METHODS)
				newError(_methods_location, "methods block not closed");
			else if (_open_blocks[i] == ROUTES)
				newError(_routes_location, "routes block not closed");
		}
	}

	if (!_parse_errors.empty())
	{
		std::cerr << "Errors in configuration file:" << std::endl;
		for (size_t i = 0; i < _parse_errors.size(); i++)
			std::cerr << "\t" << _parse_errors[i] << std::endl;
	}
	if (!errors.empty())
	{
		std::cerr << "Incomplete/missing fields in configuration file:" << std::endl;
		for (size_t i = 0; i < errors.size(); i++)
			std::cerr << "\t" << errors[i] << std::endl;
	}
	if (!_parse_errors.empty() || !errors.empty())
		throw ConfigErrorException();
}



bool	Webserv::checkErrorExists(ErrorPage& error)
{
	for (size_t i = 0; i < _servers.back()->getErrorPages().size(); i++)
	{
		if (_servers.back()->getErrorPages()[i].code == error.code)
			return true;
	}
	return false;
}

bool	Webserv::checkMethodExists(std::string& method)
{
	if (::isInVector(_servers.back()->getMethods(), method))
		return true;
	return false;
}

bool	Webserv::checkRouteExists(Route& route)
{
	for (size_t i = 0; i < _servers.back()->getRoutes().size(); i++)
	{
		if (_servers.back()->getRoutes()[i].method == route.method && _servers.back()->getRoutes()[i].path == route.path)
			return true;
	}
	return false;
}



void	Webserv::newError(unsigned int line_number, std::string error)
{
	_parse_errors.push_back("\033[1;37m" + _configFile + ":" + ::itos(line_number) + "\033[0m: \033[1;31merror:\033[0m " + error);
}

std::vector<Server*>	Webserv::getServers() const
{
	return _servers;
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

const char *Webserv::FileOpenException::what() const throw()
{
	return "Couldn't open file";
}

const char *Webserv::ConfigErrorException::what() const throw()
{
	return "Errors in configuration file";
}

const char *Webserv::ConfigIncompleteException::what() const throw()
{
	return "Incomplete configuration file";
}
