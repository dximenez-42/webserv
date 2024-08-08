#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../../includes.hpp"

class Request {
	private:
		std::string _method;
		std::string _uri;
		std::string _normalizedUri;
		std::string _http_version;
		std::string _content_type;
		std::string _content_boundary;
		std::string _content_length;
		std::vector<FormField> _form;
		std::string _body;

		FormField form;
		bool in_form;
		bool in_body;
		bool empty_line;

	public:
		Request();
		Request(const Request& src);
		Request& operator=(const Request& rhs);
		~Request();

		void printRequest();
		void fillRequest(std::string str);
		std::string getMethod() const;
		std::string getUri() const;
		std::string getNormalizedUri() const;
		std::string getHttpVersion() const;
		std::string getContentType() const;
		std::string getContentLength() const;
		std::vector<FormField> getForm() const;
		std::string getBody() const;
		std::string getBoundary() const;
		std::string getHeaderValue(const std::string& key) const;
};


#endif
