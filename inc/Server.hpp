#pragma once

#include "main.hpp"
#include PARSE_HPP
#include CLIENT_HPP
#include CONNECTION_HPP
#include UTILS_HPP
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/ip.h>

class Server {
public:
	serverConfigs					*serverConfigs;
	std::vector<Connection*>		S_CONNS;
	// Current
	size_t							curIdx;
	Connection						*curConnec;
	Client							*curClient;
	int								curFd;

	// ##########################################
	// SETUP
	// ##########################################

    Server( char *confFileName );
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

    serverRoute		findRoute( std::string path );
	void		ERROR( int status_code ) {
		curClient->RES->status_code = status_code;
		std::string content;
		if (serverConfigs->errorPages.find(status_code) != serverConfigs->errorPages.end()) {
			content = readFileContent(serverConfigs->errorPages[status_code]);
		} else {
			content = intToChar(status_code) + " " + *getStatusMsg(status_code);
		}
		curClient->RES->addPayload(content);
	};

};
