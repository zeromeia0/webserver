#include "Server.hpp"

/* Each connection contains the poll_fd,
but we must send only poll_fds vector to the poll() function,
so we're recreating the poll_fds vector, request poll(),
then we re-updated the pollFds in each connection. */
void Server::POLL() {
	std::vector<pollfd> tmp;
	for (std::vector<Connection*>::iterator it = S_CONNS.begin(); it != S_CONNS.end(); ++it) {
		tmp.push_back((*it)->pollFd);
	}
	poll(tmp.data(), tmp.size(), 1000);
	for (size_t i = 0; i < S_CONNS.size(); i++)
		S_CONNS[i]->pollFd.revents = tmp[i].revents;
}

// bool Server::checkTimeout() {

// }

void Server::LOOP() {
	LOG("DEBUG", __FUNCTION__);
    while (G_RUNNING) {

		POLL();

		time_t now = time(NULL);
		for (curIdx = 0; curIdx < S_CONNS.size(); curIdx++) {
			curConnec	= S_CONNS[curIdx];
			curClient	= curConnec->client;
			if (curClient && (now - curConnec->lastActive) > TIMEOUT) {
				curClient->RES	= new Response(curClient->REQ);
				STATUS(408);
				curClient->RES->stringify();
				send(curConnec->pollFd.fd, curClient->RES->body.c_str(), curClient->RES->body.size(), 0);
				close(curConnec->pollFd.fd);
				S_CONNS.erase(S_CONNS.begin() + curIdx);
				curIdx--;
			}
		}

		LOG("# CONNS", S_CONNS.size());
		for (curIdx = 0; curIdx < S_CONNS.size(); curIdx++) {

			/* Define current to simplify use through server execution */
			curConnec	= S_CONNS[curIdx];
			curClient	= curConnec->client;
			curFd		= curConnec->pollFd.fd;

			/* Either revents POLLIN (req) or POLLOUT (res) */
			if (curConnec->pollFd.revents & POLLIN) {
				LOG("DEBUG", "POLLIN");
				/* If the conn has no client, it means it is a connection
				linked to the server listening ports, not a client.
				So we create a new connection/client */
				if (!curClient) {
					Connection *new_conn = new Connection(curFd);
					new_conn->client		= new Client(new_conn->pollFd.fd);
					new_conn->client->REQ	= new Request;
					S_CONNS.push_back(new_conn);
				} else {
					IN();
					if (curConnec->client->REQ->payloadLen > serverConfigs->clientMaxBodySize) {
						curClient->RES	= new Response(curClient->REQ);
						STATUS(413);
						curClient->RES->stringify();
						send(curConnec->pollFd.fd, curClient->RES->body.c_str(), curClient->RES->body.size(), 0);
						close(curConnec->pollFd.fd);
						S_CONNS.erase(S_CONNS.begin() + curIdx);
						curIdx--;		
					}
					LOG("DEBUG", "HELLO");
				}
			} else if (curConnec->pollFd.revents & POLLOUT) {
				LOG("DEBUG", "POLLOUT");
				curClient->RES	= new Response(curClient->REQ);
				OUT();
				close(curConnec->pollFd.fd);
				S_CONNS.erase(S_CONNS.begin() + curIdx);
				curIdx--;				
			}
		}
	}
}
