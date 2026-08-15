#include "Server.hpp"

Server::Server() {
	serverConfigs = NULL;
	curIdx = 0;
	curConnec = NULL;
	curClient = NULL;
	curFd = -1;
}

Server::Server( char *confFileName ) {
	LOG("DEBUG", __FUNCTION__);
	signal(SIGPIPE, SIG_IGN);
	serverConfigs = parseConfigs(confFileName);

}

Server::Server( const Server &other ) {
	*this = other;
}

Server &Server::operator=( const Server &other ) {
	if (this != &other) {
		this->serverConfigs = other.serverConfigs;
		this->S_CONNS = other.S_CONNS;
		this->curIdx = other.curIdx;
		this->curConnec = other.curConnec;
		this->curClient = other.curClient;
		this->curFd = other.curFd;
	}
	return (*this);
}

Server::~Server() {
	_free<sConfigs>(serverConfigs);
	_free<Connection>(curConnec);
	_free<Client>(curClient);
}
