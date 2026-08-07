#pragma once

#include "_main.hpp"
#include CLIENT_HPP
#include <poll.h>

class Connection {
public:
	pollfd			pollFd;
	Client			*client;

	// OCF
	Connection();
	// Connection( const Connection &other);
	// Connection &operator=( const Connection &other);
	~Connection();
};
