#include <iostream>
#include "includes.hpp"
#include "src/Webserv/Webserv.hpp"
#include "src/Request/Request.hpp"
#include <csignal>

char *post_request_form2()
{
	char *request = "POST /form HTTP/1.1\r\nUser-Agent: PostmanRuntime/7.40.0\r\nAccept: */*\r\nPostman-Token: 3a3f6886-dde6-4b77-9ac4-cfe2279cf2c3\r\nHost: localhost:8085\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 49\r\n\r\nname=dani%0Ahola%0A%0Aque%20tal&username=dximenez";
	return request;
}

int	main(int ac, char **av, char **envp)
{

	(void)envp;
	if (ac != 2)
	{
		std::cout << "Error: usage ./webserv <configuration file>" << std::endl;
		return EXIT_FAILURE;
	}
	if (std::string(av[1]).find(".cfg") == std::string::npos)
	{
		std::cout << "Error: configuration file must have .cfg extension" << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		std::string	filename = av[1];
		Webserv		webserv = Webserv(filename);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception thrown: " << e.what() << std::endl;
	}
	

	return EXIT_SUCCESS;
}