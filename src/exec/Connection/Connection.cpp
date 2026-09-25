#include "Connection.hpp"

void Connection::init() {
	port = 0;
	conf = NULL;
	pollFd.fd = -1;
	pollFd.events = 0;
	pollFd.revents = 0;
	lastActive = time(NULL);
	client = NULL;
	parent = NULL;
	cgi_state = DEFAULT;
	cgi_offset = 0;
	transfer_type = CONTENT;
	contentLen = 0;
}

Connection::Connection() {
	init();
	type = SERVER;
};

Connection::Connection( unsigned int nPort, int fd, CONNECTION_TYPE newType, Connection *newParent ) {
	init();
	type = newType;
	pollFd.fd = fd;
	port = nPort;

	switch (type) {
		case SERVER:
			break;
		case CLIENT:
			client = new Client;
			pollFd.events = POLLIN;
			break;
		case CGI_IN:
			pollFd.events = POLLOUT;
			fcntl(fd, F_SETFL, O_NONBLOCK);
			parent = newParent;
			break;
		case CGI_OUT:
			pollFd.events = POLLIN;
			fcntl(fd, F_SETFL, O_NONBLOCK);
			parent = newParent;
			break;
	}
};

void Connection::updateLastActive() {
	lastActive = time(NULL);
	if (parent)
		parent->lastActive = time(NULL);
};

Connection::~Connection() {
	_free<Client>(client);
};
