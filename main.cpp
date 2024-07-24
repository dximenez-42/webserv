#include <iostream>
#include "includes.hpp"
#include "src/Server/Server.hpp"

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
		Server		server(filename);
		server.parseConfig();
		server.printConfig();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Exception thrown: " << e.what() << std::endl;
	}

	ServerList ServerList;

	ServerList.config(av[1]);
	if (ServerList.setup() == -1)
		return(1);
	ServerList.run();


	return EXIT_SUCCESS;
}