NAME		= webserv
CXX			= c++
CXXFLAGS	= -g3 -std=c++11 #-Wall -Wextra -Werror

SRCS		=	main.cpp\
				src/Webserv/Webserv.cpp\
				src/AConfig/AConfig.cpp\
				src/Server/Server.cpp\
				src/Request/Request.cpp

OBJS		= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

1: all clean
	clear
	@echo "\033[1;36mRunning server with config file:\033[0m \033[0;32mconfigs/server.cfg\033[0m  \n"
	./$(NAME) configs/server.cfg
