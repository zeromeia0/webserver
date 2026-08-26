#pragma once

#include "#MAIN.hpp"
#include CLIENT_HPP
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>

class Connection {
public:
	pollfd		pollFd;
	time_t		lastActive;
	Client		*client;

	Connection();
	Connection( int fd );
	Connection( const Connection &other);
	Connection &operator=( const Connection &other);
	~Connection();
};
