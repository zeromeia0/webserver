#include "Response.hpp"

Response::Response() : Re(RES) {
	statusCode = 0;
};

Response::Response( const Response &other ) : Re(other) {
	*this = other;
};

Response &Response::operator=( const Response &other ) {
	if (this != &other) {
		Re::operator=(other);
		this->statusCode = other.statusCode;
	}
	return (*this);
};

Response::~Response() {

};

std::string Response::stringify() {
	std::string body = "HTTP/1.1 " + intToChar(statusCode) + " " + getStatusMsg(statusCode) + "\r\n";
	for (std::map<std::string, std::string>::iterator it = this->headers.headers.begin(); it != this->headers.headers.end(); ++it)
		body = body + it->first + ": " + it->second + "\r\n";
	return (body + "\r\n" + payload);
};	
