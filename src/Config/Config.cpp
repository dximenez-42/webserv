#include "Config.hpp"

Config::Config(std::string& filename)
{
	_filename = filename;
	_input.open(filename, std::ifstream::in);
	if (!_input.good() || !_input.is_open())
		throw FileOpenException();
	
	_access_log = "";
	_error_log = "";
	_server_port = 0;
	_server_host = "";
	_server_name = "default_name";
	_server_index = "";
	_server_dir_listing = -1;
	_server_root = "";
	_server_public = "";
	_limit_body_size = 0;
}

Config::Config(Config const &src)
{
	*this = src;
}

Config& Config::operator=(Config const &rhs)
{
	if (this != &rhs)
	{
		*this = rhs;
	}
	return *this;
}

Config::~Config()
{
	if (_input.is_open())
		_input.close();
	if (!_parse_errors.empty())
		_parse_errors.clear();
	
	if (!_error_pages.empty())
		_error_pages.clear();
	if (!_methods.empty())
		_methods.clear();
	if (!_routes.empty())
		_routes.clear();
}

void	Config::parseHttp(std::vector<std::string>& split, unsigned int line_number)
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

void	Config::parseServer(std::vector<std::string>& split, unsigned int line_number)
{
	if (split.size() != 2)
		return newError(line_number, "invalid line in server block");
	if (_open_blocks.size() != 2 || _open_blocks.at(0) != HTTP || _open_blocks.at(1) != SERVER)
	{
		_open_blocks.push_back(BAD);
		return newError(line_number, "invalid server block nesting");
	}

	if (split[0] == "port" && _server_port == 0)
	{
		_server_port = std::stoi(split[1]);
		if (_server_port <= 0)
			return newError(line_number, "port must be greater than 0");
	}
	else if (split[0] == "port" && _server_port != 0)
		return newError(line_number, "port already defined");
	else if (split[0] == "host" && _server_host.empty())
	{
		_server_host = split[1];
	}
	else if (split[0] == "host" && !_server_host.empty())
		return newError(line_number, "host already defined");
	else if (split[0] == "name" && _server_name == "default_name")
	{
		_server_name = split[1];
	}
	else if (split[0] == "name" && _server_name != "default_name")
		return newError(line_number, "name already defined");
	else if (split[0] == "index" && _server_index.empty())
	{
		_server_index = split[1];
	}
	else if (split[0] == "index" && !_server_index.empty())
		return newError(line_number, "index already defined");
	else if (split[0] == "dir_listing" && _server_dir_listing == -1)
	{
		if (split[1] == "true")
			_server_dir_listing = 1;
		else if (split[1] == "false")
			_server_dir_listing = 0;
	}
	else if (split[0] == "dir_listing" && _server_dir_listing != -1)
		return newError(line_number, "dir_listing already defined");
	else if (split[0] == "root" && _server_root.empty())
	{
		_server_root = split[1];
	}
	else if (split[0] == "root" && !_server_root.empty())
		return newError(line_number, "root already defined");
	else if (split[0] == "public" && _server_public.empty())
	{
		_server_public = split[1];
	}
	else if (split[0] == "public" && !_server_public.empty())
		return newError(line_number, "public already defined");
	else if (split[0] == "limit_body_size" && _limit_body_size == 0)
	{
		_limit_body_size = std::stoi(split[1]);
		if (_limit_body_size <= 0)
			return newError(line_number, "limit_body_size must be greater than 0");
	}
	else if (split[0] == "limit_body_size" && _limit_body_size != 0)
		return newError(line_number, "limit_body_size already defined");
	else
		return newError(line_number, "invalid field \"" + split[0] + "\" in server block");
}

bool	Config::checkErrorExists(ErrorPage& error)
{
	for (size_t i = 0; i < _error_pages.size(); i++)
	{
		if (_error_pages[i].code == error.code)
			return true;
	}
	return false;
}

bool	Config::checkMethodExists(std::string& method)
{
	if (std::find(_methods.begin(), _methods.end(), method) != _methods.end())
		return true;
	return false;
}

bool	Config::checkRouteExists(Route& route)
{
	for (size_t i = 0; i < _routes.size(); i++)
	{
		if (_routes[i].method == route.method && _routes[i].path == route.path)
			return true;
	}
	return false;
}

void	Config::parseErrors(std::vector<std::string>& split, unsigned int line_number)
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
	_error_pages.push_back(error);
}

void	Config::parseMethods(std::vector<std::string>& split, unsigned int line_number)
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
	_methods.push_back(split[0]);
}

void	Config::parseRoutes(std::vector<std::string>& split, unsigned int line_number)
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
	_routes.push_back(route);
}

void	Config::parseLine(std::string& line, unsigned int line_number)
{
	std::vector<std::string>	split = Utils::split_spaces(line);
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

void	Config::parseConfig()
{
	unsigned int				line_number = 0;
	std::string					line;

	while (std::getline(_input, line))
	{
		++line_number;
		if (line.empty() || Utils::onlySpaces(line))
			continue;
		parseLine(line, line_number);
	}
	checkConfig();
	//TODO clear _parse_errors & _open_blocks
}

void	Config::checkConfig()
{
	std::vector<std::string>	errors;

	if (_access_log.empty())
		errors.push_back("access_log");
	if (_error_log.empty())
		errors.push_back("error_log");
	if (_server_port == 0)
		errors.push_back("port");
	if (_server_host.empty())
		errors.push_back("host");
	if (_server_index.empty())
		errors.push_back("index");
	if (_server_dir_listing == -1)
		errors.push_back("dir_listing");
	if (_server_root.empty())
		errors.push_back("root");
	if (_server_public.empty())
		errors.push_back("public");
	if (_limit_body_size == 0)
		errors.push_back("limit_body_size");
	if (_methods.empty())
		errors.push_back("methods");

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

void	Config::printConfig()
{
	std::cout << "access_log:\t\t" << _access_log << std::endl;
	std::cout << "error_log:\t\t" << _error_log << std::endl << std::endl;

	std::cout << "server_port:\t\t" << _server_port << std::endl;
	std::cout << "server_name:\t\t" << _server_name << std::endl;
	std::cout << "server_index:\t\t" << _server_index << std::endl;
	std::cout << "server_dir_listing:\t" << _server_dir_listing << std::endl;
	std::cout << "server_root:\t\t" << _server_root << std::endl;
	std::cout << "server_public:\t\t" << _server_public << std::endl;
	std::cout << "limit_body_size:\t" << _limit_body_size << std::endl << std::endl;

	if (!_error_pages.empty())
	{
		std::cout << "error_pages:" << std::endl;
		for (size_t i = 0; i < _error_pages.size(); i++)
		{
			std::cout	<< "\terror_code: \"" << _error_pages[i].code
						<< "\", error_page: \"" << _error_pages[i].path << "\"" << std::endl;
		}
	}

	if (!_methods.empty())
	{
		std::cout << "methods:" << std::endl;
		for (size_t i = 0; i < _methods.size(); i++)
			std::cout << "\t" << _methods[i] << std::endl;
	}

	if (!_routes.empty())
	{
		std::cout << "routes:" << std::endl;
		for (size_t i = 0; i < _routes.size(); i++)
		{
			std::cout	<< "\tmethod: \"" << _routes[i].method
						<< "\", path: \"" << _routes[i].path
						<< "\", location: \"" << _routes[i].location << "\"" << std::endl;
		}
	}
}

void	Config::newError(unsigned int line_number, std::string error)
{
	_parse_errors.push_back("\033[1;37m" + _filename + ":" + std::to_string(line_number) + "\033[0m: \033[1;31merror:\033[0m " + error);
}

const char *Config::FileOpenException::what() const throw()
{
	return "Couldn't open file";
}

const char *Config::ConfigErrorException::what() const throw()
{
	return "Errors in configuration file";
}

const char *Config::ConfigIncompleteException::what() const throw()
{
	return "Incomplete configuration file";
}
