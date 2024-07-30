#include <iostream>
#include "includes.hpp"
#include "src/Webserv/Webserv.hpp"

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
		webserv.printServers();
		webserv.setUpServers();
		webserv.runServers();

	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception thrown: " << e.what() << std::endl;
	}
	

	return EXIT_SUCCESS;
}