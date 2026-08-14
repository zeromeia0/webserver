#pragma once

#include "_main.hpp"
#include PARSE_HPP
#include EXEC_HPP
#include DEBUG_HPP
#include CONNECTION_HPP
#include UTILS_HPP
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/ip.h>

class Server {
public:
	serverConfigs					*serverConfigs;
	std::vector<Connection*>		S_CONNS;

	/*To reduce the path to a certain object,
	we created these current parameters to use
	throughout the server execution. */
	size_t							curIdx;
	Connection						*curConnec;
	Client							*curClient;
	int								curFd;

    Server( char *configsFilename );
    Server &operator=( const Server &other );
    Server( const Server &other );
    ~Server();

	void						setupServer();
	void						setOptions();
	void						bindSocket( int port );
	void						listenSocket();
	bool						checkSetup();
	
    void						START();
	void						LOOP();
	bool						POLL();
	void                        IN();
	void                        OUT();
	void						END();

	void						STATUS( int status_code );

};
