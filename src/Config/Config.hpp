#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../../includes.hpp"

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

enum OpenCfg
{
	NONE,
	HTTP,
	SERVER,
	ERRORS,
	METHODS,
	ROUTES
};

class Config
{
	private:
		std::string		_filename;
		std::ifstream	_input;
		std::vector<std::string>	_parse_errors;

		std::string		_access_log;
		std::string		_error_log;

		int				_server_port;
		std::string		_server_host;
		std::string		_server_name;
		std::string		_server_index;
		short			_server_dir_listing;
		std::string		_server_root;
		std::string		_server_public;
		int				_limit_body_size;
		
		std::vector<ErrorPage>		_error_pages;
		std::vector<std::string>	_methods;
		std::vector<Route>			_routes;

		void	parseLine(std::string& line, unsigned int line_number);

		void	parseHttp(std::vector<std::string>& split, unsigned int line_number);
		void	parseServer(std::vector<std::string>& split, unsigned int line_number);
		void	parseErrors(std::vector<std::string>& split, unsigned int line_number);
		void	parseMethods(std::vector<std::string>& split, unsigned int line_number);
		void	parseRoutes(std::vector<std::string>& split, unsigned int line_number);

		void	newError(unsigned int line_number, std::string error);

		void	checkConfig();

	public:
		Config(std::string& filename);
		Config(Config const &src);
		Config& operator=(Config const &rhs);
		~Config();

		void	parseConfig();
		void	printConfig();

		class FileOpenException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		class ConfigErrorException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
		class ConfigIncompleteException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif