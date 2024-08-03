#include <iostream>
#include "includes.hpp"
#include "src/Webserv/Webserv.hpp"
#include "src/Request/Request.hpp"
#include <csignal>

char *get_request()
{
	char *request = "GET /wp-login.php HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nsec-ch-ua: \"Chromium\";v=\"124\", \"Google Chrome\";v=\"124\", \"Not-A.Brand\";v=\"99\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Linux\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nAccept-Encoding: gzip, deflate, br, zstd\r\nAccept-Language: en-GB,en-US;q=0.9,en;q=0.8\r\nCookie: wp-settings-time-2=1720540878; wp-settings-time-1=1720545534";
	return request;
}

char *post_request()
{
	char *request = "POST /form HTTP/1.1\r\nContent-Type: multipart/form-data; boundary=--------------------------463814737340599049118228\r\nContent-Length: 292\r\n\r\n----------------------------463814737340599049118228\r\nContent-Disposition: form-data; name=\"name\"\r\n\r\ndani\r\nhola\r\n----------------------------463814737340599049118228\r\nContent-Disposition: form-data; name=\"surname\"\r\n\r\nximenez\r\n\r\nde\r\n\r\nembun\r\n----------------------------463814737340599049118228--";
	return request;
}

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

		// webserv.printServers();

		//Request	req(post_request());
		//req.printRequest();
		
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