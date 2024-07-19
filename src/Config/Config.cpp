#include "Config.hpp"

Config::Config(std::string& filename)
{
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
	if (split[0] == "access_log")
		_access_log = split[1];
	else if (split[0] == "error_log")
		_error_log = split[1];
	else
		_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid field in http block");
}

void	Config::parseServer(std::vector<std::string>& split, unsigned int line_number)
{
	if (split[0] == "port")
	{
		_server_port = std::stoi(split[1]);
	}
	else if (split[0] == "host")
	{
		_server_host = split[1];
	}
	else if (split[0] == "name")
	{
		_server_name = split[1];
	}
	else if (split[0] == "index")
	{
		_server_index = split[1];
	}
	else if (split[0] == "dir_listing")
	{
		if (split[1] == "true")
			_server_dir_listing = 1;
		else if (split[1] == "false")
			_server_dir_listing = 0;
	}
	else if (split[0] == "root")
	{
		_server_root = split[1];
	}
	else if (split[0] == "public")
	{
		_server_public = split[1];
	}
	else if (split[0] == "limit_body_size")
	{
		_limit_body_size = std::stoi(split[1]);
		if (_limit_body_size <= 0)
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": limit_body_size must be greater than 0");
	}
	else
		_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid field in server block");
}

void	Config::parseErrors(std::vector<std::string>& split, unsigned int line_number)
{
	ErrorPage	error;
	error.code = split[0];
	error.path = split[1];
	_error_pages.push_back(error);
}
void	Config::parseMethods(std::vector<std::string>& split, unsigned int line_number)
{
	_methods.push_back(split[0]);
}

void	Config::parseRoutes(std::vector<std::string>& split, unsigned int line_number)
{
	Route	route;

	route.method = split[0];
	route.path = split[1];
	route.location = split[2];
	_routes.push_back(route);
}

void	Config::parseLine(std::string& line, unsigned int line_number)
{
	std::vector<std::string>	split = Utils::split_spaces(line);
	static bool		http = false;
	static bool		server = false;
	static bool		errors = false;
	static bool		methods = false;
	static bool		routes = false;

	if (split.size() >= 1 && split[0][0] == '#')
		return ;

	// std::cout << "line: \"" << line << "\"" << std::endl;
	// std::cout << "\tsplit.size() = " << split.size() << std::endl;
	// std::cout << "\tsplit[0] = " << split[0] << std::endl;
	// if (split.size() == 2)
	// 	std::cout << "\tsplit[1] = " << split[1] << std::endl << std::endl;
	// else
	// 	std::cout << std::endl;

	if (split.size() == 2 && split[1] == "{")
	{
		if (split[0] == "http")
			http = true;
		else if (split[0] == "http" && http)
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": http block already defined");
		else if (split[0] == "server" && http)
			server = true;
		else if (split[0] == "server" && !http)
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": server block must be defined in http block");
		else if (split[0] == "server" && server)
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": server block already defined");
		else if ((split[0] == "errors" || split[0] == "methods" || split[0] == "routes") && http && server)
		{
			if (split[0] == "errors")
				errors = true;
			else if (split[0] == "methods")
				methods = true;
			else if (split[0] == "routes")
				routes = true;
		}
		else if ((split[0] == "errors" || split[0] == "methods" || split[0] == "routes") && !server)
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ":" + split[0] + "block must be defined in server block");
		else
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid block definition");
	}
	else if (split.size() == 1 && split[0] == "}")
	{
		if (routes || methods || errors)
		{
			routes = false;
			methods = false;
			errors = false;
		}
		else if (!routes && !methods && !errors && server)
			server = false;
		else if (!routes && !methods && !errors && http)
			http = false;
		else
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid block closing");
	}
	else if (split.size() >= 1 || split.size() <= 3)
	{
		if (http && !server && split.size() == 2)
			parseHttp(split, line_number);
		else if (http && server)
		{
			if (errors && split.size() == 2)
				parseErrors(split, line_number);
			else if (methods && split.size() == 1)
				parseMethods(split, line_number);
			else if (routes && split.size() == 3)
				parseRoutes(split, line_number);
			else if (split.size() == 2)
				parseServer(split, line_number);
			else
				_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid field in server block");
		}
		else
			_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid field in http block");
	}
	else
		_parse_errors.push_back("Error in line " + std::to_string(line_number) + ": invalid line format");
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

	if (!_parse_errors.empty())
	{
		std::cerr << "Errors in configuration file:" << std::endl;
		for (size_t i = 0; i < _parse_errors.size(); i++)
			std::cerr << "\t" << _parse_errors[i] << std::endl;
	}
	if (!errors.empty())
	{
		std::cerr << "Incomplete fields in configuration file:" << std::endl;
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
