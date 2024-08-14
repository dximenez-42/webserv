#include "Request.hpp"

Request::Request()
{
	_method.clear();
	_normalizedUri.clear();
	//_basename.clear();
	_http_version.clear();
	_content_type.clear();
	_content_boundary.clear();
	_content_length.clear();
	_form.clear();
	_body.clear();

	in_form = false;
	in_body = false;
	empty_line = false;
};

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
	// std::cout << std::endl << "Request: \n" << str << std::endl << std::endl;

	std::vector<std::string>	lines = splitChar(str, '\n');

	for (size_t i = 0; i < lines.size(); i++)
	{
		std::vector<std::string> words = splitSpaces(lines[i]);
	    std::cout << lines[i] << std::endl;
		
		if (words.empty())
			continue;

		if (words[0] == "GET" || words[0] == "POST" || words[0] == "DELETE")
		{
			_method = words[0];
			_normalizedUri = normalizePath(words[1]);
			//_basename = getPathBasename(uri);
			_http_version = words[2];
		}
		else if (words[0] == "Content-Type:" && _content_type.empty())
		{
			_content_type = words[1];
			if (_content_type == "multipart/form-data;")
			{
				_content_boundary = getPairValue(words[2]);
			}
			else if (_content_type == "application/x-www-form-urlencoded")
			{
				in_form = true;
			}
			else if (_content_type == "text/plain")
			{
				in_body = true;
			}
		}
		else if (words[0] == "Content-Length:")
		{
			_content_length = words[1];
		}
		else if (words[0] == "Content-Disposition:" && words[1] == "form-data;" && _content_type == "multipart/form-data;")
		{
			form.key = removeQuotes(getPairValue(words[2]));
			if (words.size() > 3)
				form.filename = removeQuotes(getPairValue(words[3]));
			in_form = true;
		}
		else if (in_form && _content_type == "multipart/form-data;" && !_content_length.empty())
		{
			if (words[0] == "Content-Type:")
				continue;
			if (lines[i].empty() && !empty_line)
			{
				empty_line = true;
				continue;
			}
			if (std::string(lines[i]).find(_content_boundary) != std::string::npos)
			{
				_form.push_back(form);
				form.key.clear();
				form.value.clear();
				in_form = false;
				empty_line = false;
			}
			else
			{
				if (!form.value.empty())
					form.value.append("\n");
				form.value.append(lines[i]);
			}
		}
		else if (in_form && _content_type == "application/x-www-form-urlencoded" && !_content_length.empty())
		{
			if (lines[i].empty() && !empty_line)
			{
				empty_line = true;
				continue;
			}
			std::vector<std::string>	form_data = splitChar(lines[i], '&');

			for (size_t i = 0; i < form_data.size(); i++)
			{
				form.key = getPairKey(form_data[i]);
				form.value = getPairValue(form_data[i]);
				_form.push_back(form);
				form.key.clear();
				form.value.clear();
			}
			in_form = false;
			empty_line = false;
		}
		else if (in_body && !_content_length.empty())
		{
			if (lines[i].empty() && !empty_line)
			{
				empty_line = true;
				continue;
			}

			if (!_body.empty())
				_body.append("\n");
			_body.append(lines[i]);
		}

		words.clear();
	}
	lines.clear();
}

std::string Request::getBoundary() const {
    return _content_boundary;
}

std::string Request::getHeaderValue(const std::string& key) const {
    if (key == "Content-Type") {
        return _content_type;
    } else if (key == "Content-Length") {
        return _content_length;
    }
    return "";
}

void	Request::printRequest()
{
	std::cout << std::endl << std::endl << "Method: " << _method << std::endl;
	std::cout << "Dirname: " << _normalizedUri << std::endl;
	//std::cout << "Basename: " << _basename << std::endl;
	std::cout << "HTTP Version: " << _http_version << std::endl;
	std::cout << "Content-Type: " << _content_type << std::endl;
	std::cout << "Content-Length: " << _content_length << std::endl << std::endl;

	if (!_form.empty())
	{
		std::cout << "Form:" << std::endl;
		for (size_t i = 0; i < _form.size(); i++)
		{
			std::cout << "\tKey: " << _form[i].key << std::endl;
			std::cout << "\tValue: " << _form[i].value << std::endl;
			std::cout << "\tFilename: " << _form[i].filename << std::endl;
		}
	}
	std::cout << std::endl << std::endl << std::endl;

}

std::string				Request::getMethod() const 
{
	return _method;
};

std::string		Request::getBasename() const
{
	return "BORRAR BASENAME";
	//return _basename;
};

std::string				Request::getNormalizedUri() const
{
	return _normalizedUri;
};

std::string				Request::getHttpVersion() const
{
	return _http_version;
};

std::string				Request::getContentType() const
{
	return _content_type;
};

std::string				Request::getContentLength() const
{
	return _content_length;
};

std::vector<FormField>	Request::getForm() const
{
	return _form;
};

std::string				Request::getBody() const
{
	return _body;
};
