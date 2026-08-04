#pragma once
#include "../Re/Request.hpp"
#include "../Re/Response.hpp"

enum C_STATE {
	READING_HEADERS,
    READING_PAYLOAD,
    COMPLETED,
};

class Client {
public:
	int				fd;
	C_STATE			state;
	Request			*REQ;
    Response		*RES;

    // OCF
    Client();
    Client( int newFd );
    Client( const Client &other );
    Client &operator=( const Client &other );
    ~Client();

    void _reset();
};
