#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

class Utils
{
	private:
		Utils();
	public:
		Utils(const Utils& src);
		Utils& operator=(const Utils& rhs);
		~Utils();
		static std::vector<std::string>	split_spaces(std::string& str);
		static bool						onlySpaces(std::string& str);
};


#endif