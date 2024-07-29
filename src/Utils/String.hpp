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

template <typename T>
bool	isValidRelativePath(T& str)
{
	if (str.empty())
		return false;
	if (str[0] == '/')
		return false;
	return isValidPath(str);
}

/**
 * @brief Check is valid and exists (does not check permissions!)
 * @param str The path to check
 * @return true if the path is valid, false otherwise
 */
template <typename T>
bool	isValidPath(T& str)
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
std::string	itos(T number)
{
	std::string	result;
	std::string	tmp;

	if (number == 0)
		return "0";
	if (number == -2147483648)
		return "-2147483648";
	if (number < 0)
	{
		result += '-';
		number *= -1;
	}
	while (number > 0)
	{
		tmp = number % 10 + '0';
		result = tmp + result;
		number /= 10;
	}
	return result;
}

#endif