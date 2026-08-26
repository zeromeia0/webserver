#pragma once

#include "#MAIN.hpp"
#include RE_HPP

class Client {
public:
	int				fd;
	CLIENT_STATE	state;
	Request			*REQ;
	Response		*RES;

	Client();
	Client( int nFd );
	Client( const Client &other );
	Client &operator=( const Client &other );
	~Client();
};
