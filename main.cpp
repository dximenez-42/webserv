#include <iostream>
#include "includes.hpp"
#include "src/Webserv/Webserv.hpp"
#include "src/Request/Request.hpp"
#include <csignal>

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    // Cerrar sockets y limpiar recursos aquí
    exit(signum);
}

int	main(int ac, char **av, char **envp)
{
    signal(SIGINT, signalHandler);

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