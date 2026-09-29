#pragma once

#include "main.hpp"
#include RE_HPP
#include PARSE_HPP
#include EXEC_HPP
#include DEBUG_HPP
#include CONNECTION_HPP
#include UTILS_HPP
#include CHECK_HPP
#include <netinet/ip.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <climits>
#include <netdb.h>

extern volatile sig_atomic_t G_RUNNING;

class Server {
private:
	Server( const Server &other );
	Server &operator=( const Server &other );
public:
	std::vector<sConfigs*>		serverConfigs;
	std::vector<Connection*>	serverConnections;
	std::vector<pid_t>			cgiPids;
	std::vector<Connection*>	newConns;

	/*To reduce the path to a certain object,
	we created these current parameters to use
	throughout the server execution. */
	int							curConnections;
	size_t						curIdx;
	Connection					*curConnec;
	Client						*curClient;
	pollfd						curPollFd;
	int							curFd;
	size_t						curContentLen;
	sRoute						curRoute;
	RE_METHOD					curMethod;

	Server();
	Server( char *configsFilename );
	~Server();

	void								init();
	void								setupServer();
	void								setOptions();
	void								bindSocket( const std::string &host, int port );
	void								listenSocket();
	void								removeZombiesCgi();
	void								addConnection();
	void								closeConnection();
	std::string							getPath();
	bool								isCgi();
	int									startCgi();
	std::map<std::string, std::string>	handleEnvp( const std::string &scriptFile );
	bool								createNewClient();
	int									sendDataToClient();
	int									connectionCheck();
	RUNTIME_ERROR						validateRequest();
	void								finishRequest();
	void								killCgi( pid_t pid );

	void								START();
	void								LOOP();
	void								POLL();
	void								OUT();
	void								STATUS( int status_code );
	void								SEND();

};
