#include "Server.hpp"

Server::Server() {
	LOG("DEBUG", __FUNCTION__);
	serverConfigs = NULL;
	curIdx = 0;
	curConnec = NULL;
	curClient = NULL;
	curFd = -1;
	curContentLen = 0;
}

Server::Server( char *confFileName ) {
	LOG("DEBUG", __FUNCTION__);
	serverConfigs = parseConfigs(confFileName);
	if (serverConfigs->clientMaxBodySize < 0)
		serverConfigs->clientMaxBodySize = DEF_MAX_BODY;
	for (size_t i = 0; i < serverConfigs->router.size(); i++) {
		if (serverConfigs->router[i].clientMaxBodySize < 0) {
			serverConfigs->router[i].clientMaxBodySize = serverConfigs->clientMaxBodySize;
		}
	}
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
		this->curContentLen = other.curContentLen;
		this->curRoute = other.curRoute;
		this->curMethod = other.curMethod;
	}
	return (*this);
}

Server::~Server() {
	LOG("DEBUG", __FUNCTION__);
}
