#include "../main.hpp"

bool Server::checkSetup() {
	return (true);
}

void Server::setupServer() {
	LOG("DEBUG", "setupServer");
    sConns[idx].pfd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sConns[idx].pfd.fd < 0)
		throw std::out_of_range("ERROR: ALEADY IN USE OR SOMETHING LIKE THAT");
	fcntl(sConns[idx].pfd.fd, F_SETFL, O_NONBLOCK);
    sConns[idx].pfd.events = POLLIN;
	if (!checkSetup())
		throw std::out_of_range("ERROR: Setup missing XXX"); // TO IMPLEMENT TO CHECK EVRYTHING IS WELL SET
}

void Server::setOptions() {
	LOG("DEBUG", "setOptions");
    int opt = 1;
    setsockopt(sConns[idx].pfd.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Server::bindSocket( int port ) {
	LOG("DEBUG", "bindSocket");
	struct sockaddr_in sService;
    sService.sin_family = SIN_FAMILY;
    sService.sin_addr.s_addr = SIN_ADDR;
    sService.sin_port = htons(port);
    bind(sConns[idx].pfd.fd, (struct sockaddr *)&sService, sizeof(sockaddr));
}

void Server::listenSocket() {
	LOG("DEBUG", "listenSocket");
	listen(sConns[idx].pfd.fd, CONN_REQS_Q);
}

void Server::createClient() {
	LOG("DEBUG", "createClient");
	struct sConn tmp;
	tmp.pfd.fd = accept(sConns[idx].pfd.fd, NULL, NULL);
	if (tmp.pfd.fd < 0)
		return;
	fcntl(tmp.pfd.fd, F_SETFL, O_NONBLOCK);
	tmp.pfd.events = POLLIN;
	tmp.pfd.revents = 0;
	tmp.c = new Client(tmp.pfd.fd);
	sConns.push_back(tmp);
}

void Server::endConn() {
	LOG("DEBUG", "endRequest");
	close(sConns[idx].pfd.fd);
	delete sConns[idx].c;
	sConns.erase(sConns.begin() + idx);
	idx--;
}

bool Server::_poll() {
	std::vector<struct pollfd> pollFds;
	// Create the mandatory fds vector for poll
	for (std::vector<struct sConn>::iterator it = sConns.begin(); it != sConns.end(); ++it)
		pollFds.push_back(it->pfd);
	int ready = poll(pollFds.data(), pollFds.size(), -1);
	// Because pollFds are copies, we need to re-update the revents in sConns after
	for (size_t i = 0; i < sConns.size(); i++)
		sConns[i].pfd.revents = pollFds[i].revents;
	return (ready > 0 ? true : false);
}

bool Server::receiveRequest() {
	LOG("DEBUG", "receiveRequest");
	char str[BUFF_SIZE] = "";
	int bytes = recv(sConns[idx].pfd.fd, str, BUFF_SIZE, 0);
	if (bytes <= 0)
		return false;
	std::string body(str, bytes);
	LOG("DEBUG", "Bytes: " << bytes);
	LOG("DEBUG", "Str: " << str);
	sConns[idx].c->receive(body);
	return true;
}

void Server::closeServer() {
	LOG("DEBUG", "closeServer");
	delete sConf;
}

void Server::init() {

	signal(SIGPIPE, SIG_IGN);

	sConf = new serverConfig;
	sConf->listenPorts.clear();
	sConf->host.clear();
	sConf->serverName.clear();
	sConf->clientMaxBodySize = 0;
	sConf->errorPages.clear();
	sConf->router.clear();
	sConf->confFile.clear();

}

Server::Server() {
	init();
}

Server::Server( const Server &other ) { (void)other; }

Server &Server::operator=( const Server &other ) { (void)other; return (*this); }

Server::~Server() {}

Server::Server( char *confFileName ) {
	init();
	parse(sConf, confFileName);
}

void Server::start() {
	// Start server, looping through PORTS
    idx = 0;
	for (std::vector<int>::iterator it = sConf->listenPorts.begin(); it != sConf->listenPorts.end(); ++it) {
		struct sConn conn; memset(&conn.pfd, 0, sizeof(conn.pfd)); conn.c = NULL;
		sConns.push_back(conn);
		setupServer();
		setOptions();
		bindSocket(*it);
		listenSocket();
		idx++;
	}
	loopServer();
	closeServer();
}
