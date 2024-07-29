#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../Server/Server.hpp"
#include <vector>
#include <fstream>
#include <iostream>

class Webserv
{
	private:
		std::string			_configFile;
		std::ifstream	_input;

		std::vector<Server*>	_servers;

		std::vector<std::string>	_parse_errors;
		std::vector<OpenCfg>		_open_blocks;

		int		_http_location;
		int		_server_location;
		int		_errors_location;
		int		_methods_location;
		int		_routes_location;

		void	parseConfig();
		void	parseLine(std::string line, unsigned int line_number);

		void	parseHttp(std::vector<std::string>& split, unsigned int line_number);
		void	parseServer(std::vector<std::string>& split, unsigned int line_number);
		void	parseErrors(std::vector<std::string>& split, unsigned int line_number);
		void	parseMethods(std::vector<std::string>& split, unsigned int line_number);
		void	parseRoutes(std::vector<std::string>& split, unsigned int line_number);

		bool	checkErrorExists(ErrorPage& error);
		bool	checkMethodExists(std::string& method);
		bool	checkRouteExists(Route& route);

		void	checkConfig();
		void	newError(unsigned int line_number, std::string error);


		std::string	_access_log;
		std::string	_error_log;

	public:
		Webserv(std::string config_file);
		Webserv(const Webserv &webserv);
		Webserv &operator=(const Webserv &webserv);
		~Webserv();

		std::vector<Server*>	getServers() const;

		void	printServers();

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