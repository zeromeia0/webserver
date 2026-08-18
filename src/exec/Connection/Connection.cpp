#include "Connection.hpp"

Connection::Connection() {
	lastActive = time(NULL);
	memset(&pollFd, 0, sizeof(pollFd));
	client = NULL;
};

Connection::Connection( int fd ) {
	lastActive = time(NULL);
	memset(&pollFd, 0, sizeof(pollFd));
	client = NULL;
	pollFd.fd = accept(fd, NULL, NULL);
	// if (pollFd.fd < 0) { THROW ERROR
	// 	delete conn;
	// 	return (NULL);
	// }
	fcntl(pollFd.fd, F_SETFL, O_NONBLOCK);
	pollFd.events = POLLIN;
	pollFd.revents = 0;
};

Connection::Connection( const Connection &other ) {
	*this = other;
};

Connection &Connection::operator=( const Connection &other ) {
	if (this != &other) {
		this->lastActive = other.lastActive;
		this->pollFd = other.pollFd;
		this->client = other.client;
	}
	return (*this);
};

Connection::~Connection() {
	_free<Client>(client);
};
