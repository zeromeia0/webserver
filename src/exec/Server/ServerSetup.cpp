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
	LOG("DEBUG", __FUNCTION__);
	struct sockaddr_in service;
    service.sin_family = SIN_FAMILY;
    service.sin_addr.s_addr = SIN_ADDR;
    service.sin_port = htons(port);
    bind(curConnec->pollFd.fd, (struct sockaddr *)&service, sizeof(sockaddr));
}

void Server::listenSocket() {
	LOG("DEBUG", __FUNCTION__);
	listen(curConnec->pollFd.fd, CONN_REQS_Q);
}

void Server::END() {
	LOG("DEBUG", __FUNCTION__);
	delete serverConfigs;
}

Server::Server( char *confFileName ) {
	LOG("DEBUG", __FUNCTION__);
	signal(SIGPIPE, SIG_IGN);
	serverConfigs = parseConfigs(confFileName);

}
Server::Server( const Server &other ) { (void)other; }
Server &Server::operator=( const Server &other ) { (void)other; return (*this); }
Server::~Server() {}

void Server::START() {
	LOG("DEBUG", __FUNCTION__);
    curIdx = 0;
	for (std::vector<int>::iterator it = serverConfigs->listenPorts.begin(); it != serverConfigs->listenPorts.end(); ++it) {
		Connection *conn = new Connection;
		curConnec = conn;
		setupServer();
		setOptions();
		bindSocket(*it);
		listenSocket();
		S_CONNS.push_back(conn);
		curIdx++;
	}
	LOOP();
	END();
}
