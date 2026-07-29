#include "../main.hpp"

bool Server::checkSetup() {
	return (true);
}

void Server::setupServer() {
	LOG("DEBUG", "setupServer");
    sConns[idx].poll_fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sConns[idx].poll_fd.fd < 0)
		throw std::out_of_range("ERROR: ALEADY IN USE OR SOMETHING LIKE THAT");
	fcntl(sConns[idx].poll_fd.fd, F_SETFL, O_NONBLOCK);
    sConns[idx].poll_fd.events = POLLIN;
	if (!checkSetup())
		throw std::out_of_range("ERROR: Setup missing XXX"); // TO IMPLEMENT TO CHECK EVRYTHING IS WELL SET
}

void Server::setOptions() {
	LOG("DEBUG", "setOptions");
    int opt = 1;
    setsockopt(sConns[idx].poll_fd.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Server::bindSocket( int port ) {
	LOG("DEBUG", "bindSocket");
	struct sockaddr_in sService;
    sService.sin_family = SIN_FAMILY;
    sService.sin_addr.s_addr = SIN_ADDR;
    sService.sin_port = htons(port);
    bind(sConns[idx].poll_fd.fd, (struct sockaddr *)&sService, sizeof(sockaddr));
}

void Server::listenSocket() {
	LOG("DEBUG", "listenSocket");
	listen(sConns[idx].poll_fd.fd, CONN_REQS_Q);
}

void Server::createClient() {
	LOG("DEBUG", "createClient");
	struct sConn tmp;
	tmp.poll_fd.fd = accept(sConns[idx].poll_fd.fd, NULL, NULL);
	if (tmp.poll_fd.fd < 0)
		return;
	fcntl(tmp.poll_fd.fd, F_SETFL, O_NONBLOCK);
	tmp.poll_fd.events = POLLIN;
	tmp.poll_fd.revents = 0;
	tmp.client = new Client(tmp.poll_fd.fd);
	sConns.push_back(tmp);
}

void Server::endConn() {
	LOG("DEBUG", "endRequest");
	close(sConns[idx].poll_fd.fd);
	sConns.erase(sConns.begin() + idx);
	idx--;
}

bool Server::_poll() { // make pollFds a pointer if possible
	std::vector<struct pollfd> pollFds;
	// Create the mandatory fds vector for poll
	for (std::vector<struct sConn>::iterator it = sConns.begin(); it != sConns.end(); ++it)
		pollFds.push_back(it->poll_fd);
	int ready = poll(pollFds.data(), pollFds.size(), -1);
	// Because pollFds are copies, we need to re-update the revents in sConns after
	for (size_t i = 0; i < sConns.size(); i++)
		sConns[i].poll_fd.revents = pollFds[i].revents;
	return (ready > 0 ? true : false);
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
		struct sConn conn; memset(&conn.poll_fd, 0, sizeof(conn.poll_fd)); conn.client = NULL;
		sConns.push_back(conn);
		setupServer();
		setOptions();
		bindSocket(*it);
		listenSocket();
		idx++;
	}
	LOOP();
	closeServer();
}
