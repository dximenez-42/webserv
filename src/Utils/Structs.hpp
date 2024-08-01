#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

struct ErrorPage
{
	std::string		code;
	std::string		path;
};

struct Route
{
	std::string		method;
	std::string		path;
	std::string		location;
};

struct FormField
{
	std::string	key;
	std::string	value;
};


#endif