#pragma once

#include "main.hpp"
#include RE_HPP

class Client {
public:
	CLIENT_STATE	state;
	Request			*REQ;
	Response		*RES;

	Client();
	Client( const Client &other );
	Client &operator=( const Client &other );
	~Client();
};
