#pragma once

#include "#MAIN.hpp"
#include CLIENT_HPP
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>

class Connection {
private:
	Connection( const Connection &other);
	Connection &operator=( const Connection &other);
	void init();
public:
	CONNECTION_TYPE		type;
	pollfd				pollFd;
	time_t				lastActive;
	Client				*client;
	Connection			*parent;
	CGI_STATE			cgi_state;
	int					cgi_offset;
	TRANSFER_TYPE		transfer_type;
	size_t				contentLen;
	std::string			buffer;

	Connection();
	Connection( int fd, CONNECTION_TYPE newType, Connection *newParent );
	~Connection();

	void				updateLastActive();
};
