#include "Server.hpp"

void Server::POLL() {
	LOG("DEBUG", __FUNCTION__);
	/* Each connection contains the poll_fd,
	but we must send only poll_fds vector to the poll() function,
	so we're recreating the poll_fds vector, request poll(),
	then we re-updated the pollFds in each connection. */
	std::vector<pollfd> tmp;
	for (std::vector<Connection*>::iterator it = serverConnections.begin(); it != serverConnections.end(); ++it) {
		tmp.push_back((*it)->pollFd);
	}
	poll(tmp.data(), tmp.size(), 1000);
	for (size_t i = 0; i < serverConnections.size(); i++)
		serverConnections[i]->pollFd.revents = tmp[i].revents;
}

void Server::LOOP() {
	LOG("DEBUG", __FUNCTION__);
	while (G_RUNNING) {

		POLL();

		time_t now = time(NULL);
		for (curIdx = 0; curIdx < serverConnections.size(); curIdx++) {
			curConnec	= serverConnections[curIdx];
			curClient	= curConnec->client;
			if (curClient && (now - curConnec->lastActive) > TIMEOUT) {
				curClient->RES	= new Response(curClient->REQ);
				STATUS(408);
				SEND();
			}
		}

		for (curIdx = 0; curIdx < serverConnections.size(); curIdx++) {

			curConnec	= serverConnections[curIdx];
			curClient	= curConnec->client;
			curFd		= curConnec->pollFd.fd;

			if (curConnec->pollFd.revents & POLLIN) {
				if (!curClient) {
					Connection *conn = new Connection(curFd);
					conn->client = new Client(curFd);
					conn->client->REQ = new Request;
					serverConnections.push_back(conn);
				} else {
					IN();
					sRoute		ROUT = findRoute(curConnec->client->REQ->headers.path, serverConfigs->router);
					bool MethodNotAllowed = !valueInContainer<std::string>(getMethodTxt(curConnec->client->REQ->headers.method), ROUT.methods);
					bool PayloadTooLarge = curConnec->client->REQ->payloadLen > serverConfigs->clientMaxBodySize;
					if (MethodNotAllowed) {
						curClient->RES	= new Response(curClient->REQ);
						STATUS(405);
						SEND();
					} else if (PayloadTooLarge) {
						curClient->RES	= new Response(curClient->REQ);
						STATUS(413);
						SEND();
					}
				}
			} else if (curConnec->pollFd.revents & POLLOUT) {
				curClient->RES	= new Response(curClient->REQ);
				OUT();
			}
		}
	}
}
