#pragma once
#include <poll.h>

class Connection {
public:
	pollfd			pollFd;
	Client			*client;

	// OCF
	Connection() {
		memset(&pollFd, 0, sizeof(pollFd));
		client = NULL;
	};
	// Connection( const Connection &other) {};
	// Connection &operator=( const Connection &other) {};
	~Connection() {};

	// PARAMETERIZED CONSTRUCTORS
	// METHODS

};
