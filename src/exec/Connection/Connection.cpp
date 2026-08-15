#include "Connection.hpp"

Connection::Connection() {
	memset(&pollFd, 0, sizeof(pollFd));
	client = NULL;
};

Connection::Connection( const Connection &other ) {
	*this = other;
};

Connection &Connection::operator=( const Connection &other ) {
	if (this != &other) {
		this->pollFd = other.pollFd;
		this->client = other.client;
	}
	return (*this);
};

Connection::~Connection() {
	_free<Client>(client);
};
