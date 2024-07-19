#include "Utils.hpp"

Utils::Utils()
{}

Utils::Utils(const Utils& src)
{
	*this = src;
}

Utils& Utils::operator=(const Utils& rhs)
{
	if (this != &rhs)
	{
		*this = rhs;
	}
	return *this;
}

Utils::~Utils()
{}

std::vector<std::string>	Utils::split_spaces(std::string& str)
{
	std::vector<std::string>	result;
	std::string					tmp;

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

bool	Utils::onlySpaces(std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!std::isspace(str[i]))
			return false;
	}
	return true;
}