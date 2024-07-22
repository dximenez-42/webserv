#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <vector>

template <typename T>
bool	isInVector(std::vector<T> vec, T elem)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == elem)
			return true;
	}
	return false;
}


#endif