#include "Connection.hpp"

Connection::Connection() {
	memset(&pollFd, 0, sizeof(pollFd));
	client = NULL;
};

// Connection::Connection( const Connection &other) {};

// Connection &Connection::operator=( const Connection &other) {};

Connection::~Connection() {};
