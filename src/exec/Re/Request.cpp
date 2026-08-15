#include "Request.hpp"

Request::Request() : Re(REQ) {

};

Request::Request( const Request &other ) {
	*this = other;
};

Request &Request::operator=( const Request &other ) {
	if (this != &other) {
		Re::operator=(other);
		this->queryString = other.queryString;
	}
	return (*this);
};

Request::~Request() {

};
