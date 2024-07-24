#include "../../includes.hpp"

Cluster::Cluster() {}

Cluster::~Cluster()
{
}

void Cluster::config(std::string configFile)
{
    //Parsea el archivo de configuración
    parseConfig();
}

int Cluster::setup() {
    //Rellena un contenedor con los servidores y puertos del configFile
    std::vector<t_listen> listeners = getListeners();
    
    FD_ZERO(&_fd_set);

    //Prepara todos los servidores de la lista
    for (std::vector<t_listen>::const_iterator lstn = listeners.begin(); lstn != listeners.end(); lstn++)
    {
        Server  server(*lstn);
        long    fd;

        if (server.setup() != -1)
        {
            fd = server.getFd();
            FD_SET(fd, &_fd_set);
            _servers.insert(std::make_pair(fd, server));
            if (fd < _max_fd)
                _max_fd = fd;
            std::cout << "Setting up " << lstn->host << ":" << lstn->port << "..." << std::endl;

        }
    }
    if (_max_fd == 0)
	{
		std::cerr << "Could not setup cluster !" << std::endl;
		return (-1);
	}
	else
		return (0);
}

void Cluster::run(void) {
    std::string dot[3] = {".  ", ".. ", "..."};
    int n = 0;

    while (1) {
        fd_set reading_set;
        fd_set writing_set;
        struct timeval timeout;
        int ret = 0;

        // Prepara los conjuntos de descriptores y el timeout
        while (ret == 0) {
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            ft_memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
            FD_ZERO(&writing_set);
            for (std::vector<int>::iterator it = _ready.begin(); it != _ready.end(); it++)
                FD_SET(*it, &writing_set);

            std::cout << "\rWaiting on a connection" << dot[n++] << std::flush;
            if (n == 3)
                n = 0;

            // Espera eventos en los sockets
            ret = select(_max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
        }
    }
};