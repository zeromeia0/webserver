#include "Server.hpp"

Server::Server() {
	LOG("DEBUG", __FUNCTION__);
	serverConfigs = NULL;
	curIdx = 0;
	curConnec = NULL;
	curClient = NULL;
	curFd = -1;
}

Server::Server( char *confFileName ) {
	LOG("DEBUG", __FUNCTION__);
	serverConfigs = parseConfigs(confFileName);
	if (DEBUG)
		debugVector<std::string>(serverConfigs->confFile);
}

Server::Server( const Server &other ) {
	LOG("DEBUG", __FUNCTION__);
	*this = other;
}

Server &Server::operator=( const Server &other ) {
	LOG("DEBUG", __FUNCTION__);
	if (this != &other) {
		this->serverConfigs = other.serverConfigs;
		this->serverConnections = other.serverConnections;
		this->curIdx = other.curIdx;
		this->curConnec = other.curConnec;
		this->curClient = other.curClient;
		this->curFd = other.curFd;
	}
	return (*this);
}

Server::~Server() {
	LOG("DEBUG", __FUNCTION__);
}
