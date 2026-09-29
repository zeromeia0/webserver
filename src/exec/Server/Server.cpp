#include "Server.hpp"

void Server::init() {
	curIdx = 0;
	curConnec = NULL;
	curClient = NULL;
	curFd = -1;
	curContentLen = 0;
}

Server::Server() {
	LOG("DEBUG", __FUNCTION__);
	init();
}

Server::Server(char *confFileName) {
	LOG("DEBUG", __FUNCTION__);
	init();
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
	for (size_t i = 0; i < cgiPids.size(); i++) {
		kill(cgiPids[i], SIGKILL);
		waitpid(cgiPids[i], NULL, 0);
	}
	for (size_t i = 0; i < serverConnections.size(); i++) {
		close(serverConnections[i]->pollFd.fd);
		delete serverConnections[i];
	}
	for (size_t i = 0; i < newConns.size(); i++) {
		close(newConns[i]->pollFd.fd);
		delete newConns[i];
    }
	for (size_t i = 0; i < serverConfigs.size(); i++)
		delete serverConfigs[i];
}
