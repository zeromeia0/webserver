#pragma once
#include "../../parser/_parser.hpp"
#include "../Client/Client.hpp"
#include "../Connection/Connection.hpp"
#include "../_utils.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/ip.h>

class Server {
public:
	S_CONFIG						*S_CONFIG;
	std::vector<Connection>			S_CONNS;
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

    ROUTE		findRoute( std::string path );
	void		ERROR( int status_code ) {
		curClient->RES->status_code = status_code;
		std::string content;
		if (S_CONFIG->errorPages.find(status_code) != S_CONFIG->errorPages.end()) {
			content = readFileContent(S_CONFIG->errorPages[status_code]);
		} else {
			content = intToChar(status_code) + " " + *getStatusMsg(status_code);
		}
		curClient->RES->addPayload(content);
	};

};
