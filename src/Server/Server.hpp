#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes.hpp"


class Server : public Config
{
	private:
        long                        	_fd;
        struct sockaddr_in          	_address;
        int                         	_port;
		std::map<long, std::string> 	_requests;

	public:
		Server(std::string filename);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

		int 	accept();
        int 	setup();
        void 	setAddress();
		long 	getFd();
		void	process(long, Config &);
		int		send(long);
		int		recv(long);

		void	close(int);
		void	clean();
};


#endif