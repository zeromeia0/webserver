#include "Server.hpp"

void Server::setupServer() {
	LOG("DEBUG", __FUNCTION__);
	curConnec->pollFd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (curConnec->pollFd.fd < 0)
		THROW("Creating socket file descriptor");
	if(fcntl(curConnec->pollFd.fd, F_SETFL, O_NONBLOCK) < 0)
		THROW("Setting file descriptor status flags");
	curConnec->pollFd.events = POLLIN;
}

void Server::setOptions() {
	LOG("DEBUG", __FUNCTION__);
	int opt = 1;
	if (setsockopt(curConnec->pollFd.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		THROW("Setting options");
}

void Server::bindSocket( int port ) {
	LOG("DEBUG", __FUNCTION__ << " " << port);
	sockaddr_in service;
	service.sin_family = SIN_FAMILY;
	service.sin_addr.s_addr = SIN_ADDR;
	service.sin_port = htons(port);
	if(bind(curConnec->pollFd.fd, (sockaddr *)&service, sizeof(sockaddr)) < 0)
		THROW("Binding socket")
}

void Server::listenSocket() {
	LOG("DEBUG", __FUNCTION__);
	if (listen(curConnec->pollFd.fd, CONN_REQS_Q) < 0)
		THROW("Listening socket");
}

void Server::END() {
	LOG("DEBUG", __FUNCTION__);
	delete serverConfigs;
}

void Server::START() {
	LOG("DEBUG", __FUNCTION__);
	curIdx = 0;
	for (std::vector<int>::iterator port = serverConfigs->listenPorts.begin(); port != serverConfigs->listenPorts.end(); ++port) {
		Connection *conn = new Connection;
		curConnec = conn;
		setupServer();
		setOptions();
		bindSocket(*port);
		listenSocket();
		serverConnections.push_back(conn);
		curIdx++;
	}
	LOOP();
	END();
}
