#include "Client.hpp"

Client::Client() {
	state = READING_HEADERS;
	REQ = NULL;
	RES = NULL;
}

Client::Client( const Client &other ) {
	*this = other;
};

Client &Client::operator=( const Client &other ) {
	if (this != &other) {
		this->state = other.state;
		this->REQ = other.REQ;
		this->RES = other.RES;
	}
	return (*this);
};

Client::~Client() {
	_free<Request>(REQ);
	_free<Response>(RES);
};
