#pragma once

#include "#MAIN.hpp"
#include PARSE_HPP
#include EXEC_HPP
#include DEBUG_HPP
#include CONNECTION_HPP
#include UTILS_HPP
#include <netinet/ip.h>
#include <vector>
#include <map>
#include <cstdlib>

extern bool G_RUNNING;

class Server {
public:
	sConfigs					*serverConfigs;
	std::vector<Connection*>	serverConnections;

	/*To reduce the path to a certain object,
	we created these current parameters to use
	throughout the server execution. */
	size_t						curIdx;
	Connection					*curConnec;
	Client						*curClient;
	int							curFd;

	Server();
	Server( char *configsFilename );
	Server( const Server &other );
	Server &operator=( const Server &other );
	~Server();

	void						setupServer();
	void						setOptions();
	void						bindSocket( int port );
	void						listenSocket();

	void						START();
	void						LOOP();
	void						POLL();
	void						IN();
	void						OUT();
	void						STATUS( int status_code );
	void						SEND();
	void						END();

};
