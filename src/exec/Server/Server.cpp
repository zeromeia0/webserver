#include "Server.hpp"

Server::Server() {
	LOG("DEBUG", __FUNCTION__);
	curIdx = 0;
	curConnec = NULL;
	curClient = NULL;
	curFd = -1;
	curContentLen = 0;
}

Server::Server(char *confFileName) {
	LOG("DEBUG", __FUNCTION__);
	serverConfigs = parseConfigs(confFileName);
    for (size_t s = 0; s < serverConfigs.size(); s++) {
		sConfigs *cfg = serverConfigs[s];
		if (cfg->clientMaxBodySize < 0)
				cfg->clientMaxBodySize = DEF_MAX_BODY;
		for (size_t i = 0; i < cfg->router.size(); i++) {
			if (cfg->router[i].clientMaxBodySize < 0)
				cfg->router[i].clientMaxBodySize = cfg->clientMaxBodySize;
		}
		if (DEBUG)
			debugConfigs(cfg);
	}
}

Server::Server(const Server &other) {
	LOG("DEBUG", __FUNCTION__);
	*this = other;
}

Server &Server::operator=(const Server &other) {
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
