#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../../includes.hpp"

class Request
{
	private:
		std::string				_method;
		std::string				_uri;
		std::string				_http_version;
		std::string				_content_type;
		std::string				_content_boundary;
		std::string				_content_length;
		std::vector<FormField>	_form;
		std::string				_body;

	public:
		Request();
		//Request(std::string str);
		Request(const Request& src);
		Request& operator=(const Request& rhs);
		~Request();

		void			printRequest();
		void			fillRequest(std::string str);
		std::string				getMethod() const;
		std::string				getUri() const;
		std::string				getHttpVersion() const;
		std::string				getContentType() const;
		std::string				getContentLength() const;
		std::vector<FormField>	getForm() const;
};

#endif
