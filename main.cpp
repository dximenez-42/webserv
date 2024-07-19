#include <iostream>
#include "includes.hpp"

int	main(int ac, char **av, char **envp)
{
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
		Config		config(filename);
		config.parseConfig();
		config.printConfig();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception thrown: " << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}