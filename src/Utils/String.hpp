#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <vector>

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