#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>

template <typename T>
std::vector<T>	splitSpaces(T& str)
{
	std::vector<T>	result;
	T					tmp;

	for (size_t i = 0; i < str.length(); i++)
	{
		while (std::isspace(str[i]))
			i++;
		while (!std::isspace(str[i]) && i < str.length())
		{
			tmp += str[i];
			i++;
		}
		if (!tmp.empty())
		{
			result.push_back(tmp);
			tmp.clear();
		}
	}
	return result;
}

template <typename T>
std::vector<T>	splitChar(T& str, char c)
{
	std::vector<T>	result;
	T					tmp;

	for (size_t i = 0; i < str.length(); i++)
	{
		while (str[i] == c)
			i++;
		while (str[i] != c && i < str.length())
		{
			tmp += str[i];
			i++;
		}
		if (!tmp.empty())
		{
			result.push_back(tmp);
			tmp.clear();
		}
	}
	return result;
}

template <typename T>
bool	onlySpaces(T& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isspace(str[i]))
			return false;
	}
	return true;
}

/**
 * @brief Check is valid and exists (does not check permissions!)
 * @param str The path to check
 * @return true if the path is valid, false otherwise
 */
template <typename T>
bool	isValidPath(const T str)
{
	if (str.empty())
		return false;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '/' && str[i + 1] == '/')
			return false;
	}
	if (access(str.c_str(), F_OK) == -1)
		return false;
	return true;
}

template <typename T>
T&	normalizePath(T& src)
{
	if (src[0] == '/')
		src.erase(0, 1);
	if (src[src.size() - 1] == '/')
		src.erase(src.size() - 1, src.size());
	return src;
}

template <typename T>
bool	isValidRoutePath(const T str)
{
	if (str.empty())
		return false;
	if (str[0] != '/')
		return false;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '/' && str[i + 1] == '/')
			return false;
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= '0' && str[i] <= '9') || str[i] == '_' || str[i] == '/'))
			return false;
	}
	return true;
}

template <typename T>
bool	isHttpRoute(const T str)
{
	if (str.empty())
		return false;
	if ((str[0] == 'h' && str[1] == 't' && str[2] == 't' && str[3] == 'p' && str[4] == ':' && str[5] == '/' && str[6] == '/')
		|| (str[0] == 'h' && str[1] == 't' && str[2] == 't' && str[3] == 'p' && str[4] == 's' && str[5] == ':' && str[6] == '/' && str[7] == '/'))
		return true;
	return false;
}

template <typename T>
T	joinPaths(const T& str1, const T& str2)
{
	return str1 + "/" + str2;
}

template <typename T>
std::string	joinPaths(const std::vector<T>& paths)
{
	std::string	result;

	if (paths.size() == 1)
		return paths[0];

	for (size_t i = 0; i < paths.size(); i++)
	{
		result.append(paths[i]);
		if (i < paths.size() - 1)
			result.append("/");
	}
	return result;
}

template <typename T>
bool	isValidIp(T str)
{
	int		num = -1;
	int		dots = 0;

	if (str.empty())
		return false;
	if (str == "localhost" || str == "0.0.0.0")
		return true;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			if (num < 0)
				num = str[i] - '0';
			else
				num = num * 10 + str[i] - '0';
		}
		else if (str[i] == '.')
		{
			if (num < 0 || num > 255)
				return false;
			num = -1;
			dots++;
		}
		else
			return false;
	}
	if (num < 0 || num > 255 || dots != 3)
		return false;
	return true;
}

template <typename T>
bool	isValidFilename(T str)
{
	bool	has_text = false;
	size_t	max = str.size();

	for (size_t i = max - 1; i > 0; i--)
	{
		std::cout << str[i] << std::endl;
		if (str[i] == '.' && has_text)
			return true;
		else if (str[i] == '.' && !has_text)
			return false;
		else
			has_text = true;
	}
	return false;
}

template <typename T>
std::string	getPathDirname(T src)
{
	std::vector<T>	words = splitChar(src, '/');

	if (isValidFilename(words.back()))
		words.pop_back();
	return joinPaths(words);
}

template <typename T>
std::string	getPathBasename(T src)
{
	std::vector<T>	words = splitChar(src, '/');

	if (isValidFilename(words.back()))
		return words.back();
	return std::string();
}

template <typename T>
int	stoi(T str, int base = 10)
{
	int	result = 0;
	int	sign = 1;
	int	i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; i < str.size(); i++)
	{
		if (str[i] < '0' || str[i] > '9')
			return 0;
		result = result * base + str[i] - '0';
	}
	return result * sign;
}

template <typename T>
std::string	itos(T number, int base = 10)
{
	std::string	result;
	std::string	tmp;
	int			sign = 1;
	std::string	baseStr = "0123456789ABCDEF";

	if (number < 0)
	{
		sign = -1;
		number *= -1;
	}
	while (number > 0)
	{
		tmp += baseStr[number % base];
		number /= base;
	}
	if (sign == -1)
		tmp += '-';
	for (int i = tmp.size() - 1; i >= 0; i--)
		result += tmp[i];
	return result;
}


template <typename T>
T	getPairKey(T str)
{
	std::string	key;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '=')
			break;
		else
			key += str[i];
	}
	return key;
}

template <typename T>
T	getPairValue(T str)
{
	std::string	value;
	bool		found = false;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '=' && !found)
			found = true;
		else if (found)
			value += str[i];
	}
	return value;
}

template <typename T>
T	removeQuotes(T str)
{
	bool		first = false;
	std::string	res;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '\"' && !first)
			first = true;
		else if (str[i] == '\"' && first)
		{
			return res;
		}
		else if (first)
			res += str[i];
	}
	
	return res;
}

#endif