#include "Request.hpp"

Request::Request() : Re(REQ) {
	this->BadRequest = false;
	this->RequestTimedOut = false;
	this->MethodNotAllowed = false;
	this->PayloadTooLarge = false;
	this->MovedPermanently = false;
};

Request::Request( const Request &other ) : Re(other) {
	*this = other;
};

Request &Request::operator=( const Request &other ) {
	if (this != &other) {
		Re::operator=(other);
		this->queryString = other.queryString;
		this->BadRequest = other.BadRequest;
		this->RequestTimedOut = other.RequestTimedOut;
		this->MethodNotAllowed = other.MethodNotAllowed;
		this->PayloadTooLarge = other.PayloadTooLarge;
		this->MovedPermanently = other.MovedPermanently;
	}
	return (*this);
};

Request::~Request() {

};
