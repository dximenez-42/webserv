#include "Request.hpp"

/*Request::Request(std::string str)
{
	std::cout << "Request: \n" << str << std::endl << std::endl;


	std::vector<std::string>	lines = ::splitChar(str, '\n');
	FormField					form;
	bool						in_form = false;
	bool						form_empty_line = false;


	for (size_t i = 0; i < lines.size(); i++)
	{
		std::vector<std::string> words = ::splitSpaces(lines[i]);
		
		if (words.empty())
			continue;

		if (words[0] == "GET" || words[0] == "POST" || words[0] == "DELETE")
		{
			_method = words[0];
			_uri = words[1];
			_http_version = words[2];
		}
		else if (words[0] == "Content-Type:")
		{
			_content_type = words[1];
			if (_content_type == "multipart/form-data;")
			{
				_content_boundary = ::getPairValue(words[2]);
			}
		}
		else if (words[0] == "Content-Length:")
		{
			_content_length = words[1];
		}
		else if (words[0] == "Content-Disposition:" && words[1] == "form-data;" && _content_type == "multipart/form-data;")
		{
			form.key = ::getPairValue(words[2]);
			in_form = true;
		}
		else if (in_form)
		{
			if (lines[i].empty() && !form_empty_line)
			{
				form_empty_line = true;
				continue;
			}
			if (std::string(lines[i]).find(_content_boundary) != std::string::npos)
			{
				_form.push_back(form);
				form.key.clear();
				form.value.clear();
				in_form = false;
				form_empty_line = false;
			}
			else
			{
				if (!form.value.empty())
					form.value.append("\n");
				form.value.append(lines[i]);
			}
		}
	}
	std::cout << "Entra despues de inicializar" << std::endl;
}
*/

Request::Request(const Request& src)
{
	*this = src;
}

Request& Request::operator=(const Request& rhs)
{
	if (this != &rhs)
	{
		*this = rhs;
	}
	return *this;
}

Request::~Request()
{
	_form.clear();
}

#include <stdio.h>

void	Request::fillRequest(std::string str) {
	std::cout << "Request: \n" << str << std::endl << std::endl << std::endl << std::endl;

	std::vector<std::string>	lines = ::splitChar(str, '\n');
	FormField					form;
	bool						in_form = false;
	bool						form_empty_line = false;

	for (size_t i = 0; i < lines.size(); i++)
	{
		std::vector<std::string> words = ::splitSpaces(lines[i]);
		
		if (words.empty())
			continue;

		if (words[0] == "GET" || words[0] == "POST" || words[0] == "DELETE")
		{
			_method = words[0];
			_uri = words[1];
			_http_version = words[2];
		}
		else if (words[0] == "Content-Type:")
		{
			_content_type = words[1];
			if (_content_type == "multipart/form-data;")
			{
				_content_boundary = ::getPairValue(words[2]);
			}
		}
		else if (words[0] == "Content-Length:")
		{
			_content_length = words[1];
		}
		else if (words[0] == "Content-Disposition:" && words[1] == "form-data;" && _content_type == "multipart/form-data;")
		{
			form.key = ::getPairValue(words[2]);
			in_form = true;
		}
		else if (in_form)
		{
			if (lines[i].empty() && !form_empty_line)
			{
				form_empty_line = true;
				continue;
			}
			if (std::string(lines[i]).find(_content_boundary) != std::string::npos)
			{
				_form.push_back(form);
				form.key.clear();
				form.value.clear();
				in_form = false;
				form_empty_line = false;
			}
			else
			{
				if (!form.value.empty())
					form.value.append("\n");
				form.value.append(lines[i]);
			}
		}
	}
}

void	Request::printRequest()
{
	std::cout << std::endl << "Method: " << _method << std::endl;
	std::cout << "URI: " << _uri << std::endl;
	std::cout << "HTTP Version: " << _http_version << std::endl;
	std::cout << "Content-Type: " << _content_type << std::endl;
	std::cout << "Content-Length: " << _content_length << std::endl << std::endl;

	if (!_form.empty())
	{
		std::cout << "Form:" << std::endl;
		for (size_t i = 0; i < _form.size(); i++)
		{
			std::cout	<< _form[i].key << "="
						<< _form[i].value << std::endl;
		}
	}
}
