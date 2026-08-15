#pragma once

#include "#MAIN.hpp"
#include CLIENT_HPP
#include <poll.h>

class Connection {
public:
	pollfd		pollFd;
	Client		*client;

	Connection();
	Connection( const Connection &other);
	Connection &operator=( const Connection &other);
	~Connection();
};
