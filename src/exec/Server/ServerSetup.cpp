#include "Server.hpp"

void Server::setupServer() {
	LOG("DEBUG", __FUNCTION__);
	curConnec->pollFd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (curConnec->pollFd.fd < 0)
		THROW("Creating socket file descriptor");
	fcntl(curConnec->pollFd.fd, F_SETFL, O_NONBLOCK);
	fcntl(curConnec->pollFd.fd, F_SETFD, FD_CLOEXEC);
	curConnec->pollFd.events = POLLIN;
}

void Server::setOptions() {
	LOG("DEBUG", __FUNCTION__);
	int opt = 1;
	if (setsockopt(curConnec->pollFd.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		THROW("Setting options");
}

void Server::bindSocket( const std::string &host, int port ) {
	LOG("DEBUG", __FUNCTION__ << " " << port);
	addrinfo hints = addrinfo();
	addrinfo *res;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int err = getaddrinfo(host.empty() ? NULL : host.c_str(), intToChar(port).c_str(), &hints, &res);
	if (err != 0)
		THROW("getaddrinfo: " + std::string(gai_strerror(err)));
	int r = bind(curConnec->pollFd.fd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	if (r < 0)
		THROW("Binding socket " + host + ":" + intToChar(port));
  }

void Server::listenSocket() {
	LOG("DEBUG", __FUNCTION__);
	if (listen(curConnec->pollFd.fd, CONN_REQS_Q) < 0)
		THROW("Listening socket");
}

void Server::START() {
	LOG("DEBUG", __FUNCTION__);
	curIdx = 0;
	for (size_t s = 0; s < serverConfigs.size(); s++) {
        sConfigs *cfg = serverConfigs[s];
		for (size_t l = 0; l < cfg->listens.size(); l++) {
			Connection *conn = new Connection;
			conn->conf = cfg;
			conn->port = cfg->listens[l].second;
			serverConnections.push_back(conn);
			curConnec = conn;
			setupServer();
			setOptions();
			std::string host = cfg->listens[l].first.empty() ? cfg->host : cfg->listens[l].first;
			bindSocket(host, conn->port);
			listenSocket();
			curIdx++;
		}
	}
	LOOP();
}
