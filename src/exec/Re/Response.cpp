#include "Response.hpp"

Response::Response() : Re(RES) {
	statusCode = 0;
};

Response::Response( const Request *req ) : Re(RES) {
	headers.method = req->headers.method;
	headers.version = req->headers.version;
	headers.path = req->headers.path;
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

void Response::stringify() {
	this->body =
		this->headers.version + " "
		+ intToChar(this->statusCode) + " "
		+ *getStatusMsg(this->statusCode) + " "
		+ "\r\n";
	for (std::map<std::string, std::string>::iterator it = this->headers.headers.begin(); it != this->headers.headers.end(); ++it)
		this->body = this->body + it->first + ": " + it->second + "\r\n";
	this->body = this->body + "\r\n";
	this->body = this->body + this->payload;
};	
