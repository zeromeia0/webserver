#include "Request.hpp"

Request::Request() : Re(REQ) {
	this->rError = NONE;
};

Request::Request( const Request &other ) : Re(other) {
	*this = other;
};

Request &Request::operator=( const Request &other ) {
	if (this != &other) {
		Re::operator=(other);
		this->queryString = other.queryString;
		this->rError = other.rError;
	}
	return (*this);
};

Request::~Request() {

};
