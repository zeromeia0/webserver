#include "Server.hpp"
#include "../../_main.hpp"

/* Each connection contains the poll_fd struct,
but we must send only poll_fds vector to the poll() function,
so we're recreating the poll_fds vector, request poll(),
then we re-updated the pollFds in each connection. */
bool Server::POLL() {
	// LOG("DEBUG", __FUNCTION__);
	std::vector<struct pollfd> tmp;
	for (std::vector<Connection>::iterator it = S_CONNS.begin(); it != S_CONNS.end(); ++it)
		tmp.push_back(it->pollFd);
	int ready = poll(tmp.data(), tmp.size(), -1);
	for (size_t i = 0; i < S_CONNS.size(); i++)
		S_CONNS[i].pollFd.revents = tmp[i].revents;
	return (ready > 0 ? true : false);
}

static Connection *newConn( int fd ) {
	LOG("DEBUG", __FUNCTION__);
	Connection *conn = new Connection;
	conn->pollFd.fd = accept(fd, NULL, NULL);
	if (conn->pollFd.fd < 0) {
		delete conn;
		return (NULL);
	}
	fcntl(conn->pollFd.fd, F_SETFL, O_NONBLOCK);
	conn->pollFd.events = POLLIN;
	conn->pollFd.revents = 0;
	return (conn);
}

void Server::LOOP() {
	LOG("DEBUG", __FUNCTION__);
    while (1) {
        if (POLL()) {
			LOG("# CONNS", S_CONNS.size());
			for (curIdx = 0; curIdx < S_CONNS.size(); curIdx++) {


				/* Define current to simplify use through server execution */
				curConnec	= &S_CONNS[curIdx];
				curClient	= curConnec->client;
				curFd		= curConnec->pollFd.fd;

				/* Either revents POLLIN (req) or POLLOUT (res) */
                if (curConnec->pollFd.revents & POLLIN) {
					LOG("DEBUG", "POLLIN");
					/* If the conn has no client, it means it is a connection
					linked to the server listening ports, not a client.
					So we create a new connection/client */
					if (!curClient) {
						Connection *new_conn = newConn(curFd);
						new_conn->client		= new Client(new_conn->pollFd.fd);
						new_conn->client->REQ	= new Request;
						S_CONNS.push_back(*new_conn);
					} else {
						IN();
					}
            	} else if (curConnec->pollFd.revents & POLLOUT) {
					LOG("DEBUG", "POLLOUT");
					curClient->RES	= new Response(*curClient->REQ);
					OUT();
					close(curConnec->pollFd.fd);
					S_CONNS.erase(S_CONNS.begin() + curIdx);
					curIdx--;				
				}
			}
        }
	}
}
