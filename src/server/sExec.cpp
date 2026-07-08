#include "../main.hpp"

void Server::loopServer() {
	LOG("DEBUG", "pollLoop");
    while (1) {
        if (_poll()) {
			for (idx = 0; idx < sConns.size(); idx++) {

                if (sConns[idx].pfd.revents & POLLIN) {
					// If sConns[idx] is receiving, and it is not a client
					// we create a new client
                    if (!sConns[idx].c) {
						createClient();
						continue;
					}
					// Request is still receiving - either we respond if completed
					// or we wait for new packets
					if (receiveRequest()) {
						if (sConns[idx].c->getState() == COMPLETED)
							sConns[idx].pfd.events = POLLOUT;
					} else {
						endConn();
					}
            	}

				else if (sConns[idx].pfd.revents & POLLOUT) {
					std::string method = sConns[idx].c->REQ->getMethod();
					if (method == "GET") {
						GET(sConns[idx].pfd.fd, sConns[idx].c->REQ->getPath());
					} else if (method == "POST") {

					} else if (method == "DELETE") {

					} else {
						GET(sConns[idx].pfd.fd, std::string(F_404));
					}
					endConn();
				}

			}
        }

        LOG("DEBUG", "sConns: " << sConns.size());

	}
}


// METHODS
void Server::GET( int fd, const std::string &path ) {
	LOG("DEBUG", "respondRequest");
	Res res(path);
	send(fd, res.getBody().c_str(), res.getBody().size(), 0);
}

void Server::POST() {

}

void Server::DELETE() {

}
