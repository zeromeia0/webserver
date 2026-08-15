#include "Server.hpp"

bool Server::checkSetup() {
	LOG("DEBUG", __FUNCTION__);
	return (true);
}

void Server::setupServer() {
	LOG("DEBUG", __FUNCTION__);
    curConnec->pollFd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (curConnec->pollFd.fd < 0)
		throw std::out_of_range("ERROR: ALEADY IN USE OR SOMETHING LIKE THAT");
	fcntl(curConnec->pollFd.fd, F_SETFL, O_NONBLOCK);
    curConnec->pollFd.events = POLLIN;
	if (!checkSetup())
		throw std::out_of_range("ERROR: Setup missing XXX"); // TO IMPLEMENT TO CHECK EVRYTHING IS WELL SET
}

void Server::setOptions() {
	LOG("DEBUG", __FUNCTION__);
    int opt = 1;
    setsockopt(curConnec->pollFd.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Server::bindSocket( int port ) {
	LOG("DEBUG", __FUNCTION__ << " " << port);
	sockaddr_in service;
    service.sin_family = SIN_FAMILY;
    service.sin_addr.s_addr = SIN_ADDR;
    service.sin_port = htons(port);
    bind(curConnec->pollFd.fd, (sockaddr *)&service, sizeof(sockaddr));
}

void Server::listenSocket() {
	LOG("DEBUG", __FUNCTION__);
	listen(curConnec->pollFd.fd, CONN_REQS_Q);
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
		S_CONNS.push_back(conn);
		curIdx++;
	}
	LOOP();
	END();
}
