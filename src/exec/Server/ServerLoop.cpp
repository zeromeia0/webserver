#include "Server.hpp"

void Server::addConnection() {
	LOG("DEBUG", __FUNCTION__);

}

void Server::closeConnection() {
	LOG("DEBUG", __FUNCTION__ << " " << curFd);
	if (curConnec->type == CLIENT) {
		for (size_t i = 0; i < serverConnections.size(); i++) {
			if (serverConnections[i]->parent == curConnec) {
				close(serverConnections[i]->pollFd.fd);
				delete serverConnections[i];
				serverConnections.erase(serverConnections.begin() + i);
				if (i < curIdx)
					curIdx--;
				i--;
			}
		}
		for (size_t i = 0; i < newConns.size(); i++) {
			if (newConns[i]->parent == curConnec) {
				close(newConns[i]->pollFd.fd);
				delete newConns[i];
				newConns.erase(newConns.begin() + i);
				i--;
			}
		}
	}
	close(curFd);
	delete curConnec;
	serverConnections.erase(serverConnections.begin() + curIdx);
	curIdx--;
}

void Server::POLL() {
	// LOG("DEBUG", __FUNCTION__);
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

bool Server::isCgi() {
	LOG("DEBUG", __FUNCTION__);
	std::string PATH = getPath();
	std::string fileExtension = getFileExtension(PATH);
	return ((curRoute.cgi.find(fileExtension) != curRoute.cgi.end()) && (curMethod == POST));
}

std::map<std::string, std::string> Server::handleEnvp() {
	Request *REQ = curConnec->client->REQ;
	std::map<std::string, std::string> inputs;

	inputs["QUERY_STRING"]			= REQ->headers.query_str;
	inputs["REQUEST_METHOD"]		= getMethodTxt(REQ->headers.method);
	inputs["REQUEST_URI"]			= REQ->headers.raw;
	inputs["SCRIPT_NAME"]			= REQ->headers.script;
	inputs["PATH_INFO"]				= REQ->headers.info;
	inputs["CONTENT_TYPE"]			= REQ->getHeader("content-type");
	inputs["SERVER_PROTOCOL"]		= REQ->headers.version;

	std::string contentLength = REQ->headers.get("content-length");
	if (!contentLength.empty())
		inputs["CONTENT_LENGTH"]	= contentLength;

	std::string host = REQ->getHeader("host");
	std::string sep = ":";
	size_t sep_pos = host.find(sep);
	inputs["SERVER_NAME"]			= (sep_pos == std::string::npos) ? host : host.substr(0, sep_pos);
	inputs["SERVER_PORT"]			= (sep_pos == std::string::npos) ? "" : host.substr(sep_pos + sep.size());

	for (std::map<std::string, std::string>::iterator it = REQ->headers.headers.begin(); it != REQ->headers.headers.end(); ++it) {
		std::string name = "HTTP_" + it->first;
		for (size_t i = 5; i < name.size(); i++) {
			if (name[i] == '-')
				name[i] = '_';
			else
				name[i] = toupper(name[i]);
		}
		inputs[name]				= it->second;
	}
	if (DEBUG) {
		std::cerr << "---------- CGI ENVP ----------" << std::endl;
		debugMap<std::string, std::string>(inputs);
	}
	return (inputs);
}

int Server::startCgi() {
	LOG("DEBUG", __FUNCTION__);

	std::string PATH = getPath();
	std::string fileExtension = getFileExtension(PATH);
	std::map<std::string, std::string>::iterator it = curRoute.cgi.find(fileExtension);
	std::string cgiPath = it->second;
	
	int pipe_in[2], pipe_out[2];

	pipe(pipe_in);
	pipe(pipe_out);

	pid_t pid = fork();

	if (pid == 0) {

		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);

		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);

		char *args[] = {
			(char *)cgiPath.c_str(),
			(char *)PATH.c_str(),
			NULL
		};
		std::map<std::string, std::string> mapEnvp = handleEnvp();
		std::vector<std::string> env_strs;
		for (std::map<std::string, std::string>::iterator it = mapEnvp.begin(); it != mapEnvp.end(); ++it)
			env_strs.push_back(it->first + "=" + it->second);

		char **envp = new char*[env_strs.size() + 1];
		for (size_t i = 0; i < env_strs.size(); i++)
			envp[i] = (char *)env_strs[i].c_str();
		envp[env_strs.size()] = NULL;

		execve(cgiPath.c_str(), args, envp);
		LOG("EXECVE FAILED", "");
		_exit(1);

	}

	close(pipe_in[0]);
	close(pipe_out[1]);

	Connection *cin = new Connection(pipe_in[1], CGI_IN, curConnec);
	newConns.push_back(cin);

	Connection *cout = new Connection(pipe_out[0], CGI_OUT, curConnec);
	newConns.push_back(cout);

	return (cin->pollFd.fd);
}

bool Server::createNewClient() {
	LOG("DEBUG", __FUNCTION__);
	int clientFd = accept(curFd, NULL, NULL);
	if (clientFd < 0)
		return (false);
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
		close(clientFd);
		return (false);
	}
	Connection *newClient = new Connection(clientFd, CLIENT, NULL);
	newClient->client->REQ = new Request;
	newClient->client->RES = new Response;
	curPollFd = newClient->pollFd;
	curFd = curPollFd.fd;
	newConns.push_back(newClient);
	return (true);
}

int Server::sendDataToClient() {
	LOG("DEBUG", __FUNCTION__);
	if (curClient->RES->body.empty()) {
		if (curClient->RES->statusCode == 0) {
			curClient->RES->headers.method = curClient->REQ->headers.method;
			curClient->RES->headers.version = curClient->REQ->headers.version;
			curClient->RES->headers.path = curClient->REQ->headers.path;
		}
		if (curConnec->cgi_state == ONGOING)
			return (1);
		OUT();
		SEND();
		curClient->RES->body = curClient->RES->stringify();
	}
	size_t sizeBody = curClient->RES->body.size();
	int nbytes = send(curFd, curClient->RES->body.c_str(), sizeBody, 0);
	if (nbytes <= 0)
		return (0);
	if ((size_t)nbytes == sizeBody)
		return (0);
	curClient->RES->body = curClient->RES->body.substr(nbytes);
	return (1);
}

static int writeToPipe( int fd, const char *payload, size_t remaining ) {
	LOG("DEBUG", __FUNCTION__ << " -> Remaining: " << remaining);
	int nbytes = write(fd, payload, remaining);
	// LOG("DEBUG", "AFTER WRITE: " << nbytes);
	return (nbytes);
}

static int readFromPipe( int fd, std::string *payload ) {
	LOG("DEBUG", __FUNCTION__);
	char buff[BUFF_SIZE];
	int nbytes = read(fd, buff, BUFF_SIZE);
	LOG("DEBUG", "AFTER READ: " << nbytes);
	if (nbytes >= 0)
		payload->append(buff, nbytes);
	return (nbytes);
}

static int handleHeaders(std::string body, Connection *conn) {
	LOG("DEBUG", __FUNCTION__);
	sHeaders *parsedHeaders = parseHeaders(body);
	if (!checkHeaders(parsedHeaders))
		return (0);
	conn->client->REQ->headers = *parsedHeaders;
	delete parsedHeaders;
	return (1);
}

int Server::connectionCheck() {

	Request		*REQ;
	Client		*CLI;
	Connection	*CON;

	switch (curConnec->type) {
		case SERVER:
			return (1);
			break;
		case CLIENT:
			CON = curConnec;
			CLI = CON->client;
			REQ = CLI->REQ;
			break;
		case CGI_IN:
			CON = curConnec;
			CLI = CON->parent->client;
			REQ = CLI->REQ;
			break;
		case CGI_OUT:
			CON = curConnec;
			CLI = CON->parent->client;
			REQ = CLI->REQ;
			break;
	}

	time_t now = time(NULL);
	REQ->RequestTimedOut = (now - curConnec->lastActive) > TIMEOUT;
	if (REQ->RequestTimedOut)
		return (-1);

	if (CLI->state == READING_HEADERS)
		return (1);

	REQ->MethodNotAllowed = !valueInContainer<std::string>(getMethodTxt(REQ->headers.method), curRoute.methods);
	if (REQ->MethodNotAllowed)
		return (-2);
	REQ->PayloadTooLarge = REQ->payload.size() > (size_t)curRoute.clientMaxBodySize;
	if (REQ->PayloadTooLarge)
		return (-3);
	REQ->MovedPermanently = !curRoute.redirect.empty();
	if (REQ->MovedPermanently)
		return (-4);
	if (REQ->BadRequest)
		return (-5);
	return (1);
}

void Server::LOOP() {
	LOG("DEBUG", __FUNCTION__);

	while (G_RUNNING) {
		curConnections = serverConnections.size();
		// LOG("CONNECTIONS", curConnections);

		POLL();

		for (curIdx = 0; curIdx < serverConnections.size(); curIdx++) {

			curConnec		= serverConnections[curIdx];
			curClient		= curConnec->client;
			curPollFd		= curConnec->pollFd;
			curFd			= curPollFd.fd;
			curContentLen	= curConnec->contentLen;

			int connStatus = connectionCheck();
			// LOG("connStatus", connStatus << " fd: " << curFd);
			if (connStatus < 0) {
				if (curConnec->type == CGI_IN || curConnec->type == CGI_OUT) {
					closeConnection();
				} else if (curConnec->type == CLIENT) {
					curConnec->pollFd.events = POLLOUT;
					curConnec->cgi_state = DEFAULT;
					curClient->RES->payload.clear();
					if (sendDataToClient() == 0)
						closeConnection();
				}
				continue;
			}

			char buff[BUFF_SIZE];
			switch (curConnec->type) {
				case SERVER:
					if (curConnec->pollFd.revents & POLLIN)
						while (createNewClient()) {}
					break;
				case CLIENT:
					if (curPollFd.revents & (POLLERR | POLLHUP)) {
						closeConnection();
						break;
					}
					if (curPollFd.revents & POLLIN) {
						curConnec->updateLastActive();
						int nbytes = recv(curFd, buff, BUFF_SIZE, 0);
						if (nbytes < 0) {
							curConnec->client->REQ->BadRequest = true;
							if (curConnec->cgi_state == ONGOING)
								curConnec->cgi_state = DONE;
							curConnec->pollFd.events = POLLOUT;
							break;
						}
						if (nbytes == 0) {
							closeConnection();
							break;
						}
						buff[nbytes] = '\0';
						// LOG("DEBUG", "received buffer: " << std::string(buff));
						LOG("DEBUG", "received buffer: " << std::string(buff).substr(0, 10) << " size: " << sizeof(buff));
						if (curClient->state == READING_HEADERS) {
							curConnec->buffer.append(buff);
							size_t headersEof = curConnec->buffer.find("\r\n\r\n");
							if (headersEof != std::string::npos) {
								if(!handleHeaders(curConnec->buffer.substr(0, headersEof), curConnec)) {
									curConnec->client->REQ->BadRequest = true;
									if (curConnec->cgi_state == ONGOING)
										curConnec->cgi_state = DONE;
									curConnec->pollFd.events = POLLOUT;
									break;
								}
								if (DEBUG)
									debugRe(*curClient->REQ);
								curConnec->buffer = curConnec->buffer.substr(headersEof + 4);
								buff[0] = '\0';
								// postHeadersUpdates
								curMethod = curClient->REQ->headers.method;
								curRoute = findRoute(curConnec->client->REQ->headers.path, serverConfigs->router);
								if (curMethod == GET) {
									curClient->state = COMPLETED;
									curConnec->pollFd.events = POLLOUT;
									break;
								}
								curClient->state = READING_PAYLOAD;
								if (curClient->REQ->getHeader("transfer-encoding") == "chunked")
									curConnec->transfer_type = CHUNKED;
								if (isCgi()) {
									std::string uri = curClient->REQ->headers.raw;
									std::string file_ext = getFileExtension(uri);
									size_t ext_pos = uri.find(file_ext);
									curClient->REQ->headers.script = uri.substr(0, ext_pos + file_ext.size());
									curClient->REQ->headers.info = uri.substr(curClient->REQ->headers.script.size());
									if (curClient->REQ->headers.info.empty())
										curClient->REQ->headers.info = curClient->REQ->headers.script;
									curConnec->cgi_state = ONGOING;
									startCgi();
								}
								curConnec->contentLen = strtoul(curClient->REQ->getHeader("content-length").c_str(), NULL, 10);
								curContentLen = curConnec->contentLen;
								if (!curContentLen && curConnec->transfer_type == CONTENT) {
									curClient->state = COMPLETED;
									curConnec->pollFd.events = POLLOUT;
									// if (curConnec->cgi_state == ONGOING)
									// 	curConnec->cgi_state = DONE;
									break;
								}
							}
						}

						if (curClient->state == READING_PAYLOAD) {
							int status = 0;
							std::string new_payload;
							std::string new_bytes = buff;
							if (curConnec->transfer_type == CONTENT) {
								new_payload = curConnec->buffer + std::string(buff);
								curConnec->buffer.clear();
								if (curClient->REQ->payload.size() + new_payload.size() < curContentLen)
									status = 1;
							} else if (curConnec->transfer_type == CHUNKED)
								new_payload = parseChunkedBody(&new_bytes, &curConnec->buffer, &status);
							curClient->REQ->payload.append(new_payload);
							switch (status) {
								case 0:
									curClient->state = COMPLETED;
									curConnec->pollFd.events = POLLOUT;
									break;
								case 1:
									break;
							}
						}
					} else if (curConnec->pollFd.revents & POLLOUT) {
						curConnec->updateLastActive();
						if (curConnec->cgi_state == ONGOING)
							break;
						if (sendDataToClient() == 0)
							closeConnection();
					}
					break;
				case CGI_IN:
					if (curConnec->pollFd.revents & POLLOUT) {
						curConnec->updateLastActive();
						const char *payload = curConnec->parent->client->REQ->payload.data() + curConnec->parent->cgi_offset;
						size_t remaining = curConnec->parent->client->REQ->payload.size() - curConnec->parent->cgi_offset;
						if (!remaining) {
							if (curConnec->parent->client->state == COMPLETED)
								closeConnection();
							break;
						}
						int nbytes = writeToPipe(curFd, payload, remaining);
						if (nbytes < 0) {
							curConnec->parent->client->REQ->BadRequest = true;
							curConnec->parent->cgi_state = DONE;
							curConnec->parent->pollFd.events = POLLOUT;
							closeConnection();
							break;
						}
						curConnec->parent->cgi_offset += nbytes;
					}
					break;
				case CGI_OUT:
					if (curConnec->parent->cgi_state == ONGOING)
						curConnec->updateLastActive();
					if (curConnec->pollFd.revents & (POLLIN | POLLHUP)) {
						int nbytes = readFromPipe(curConnec->pollFd.fd, &curConnec->parent->client->RES->payload);
						if (nbytes < 0) {
							curConnec->parent->client->REQ->BadRequest = true;
							curConnec->parent->cgi_state = DONE;
							curConnec->parent->pollFd.events = POLLOUT;
							closeConnection();
							break;
						}
						if (nbytes == 0) {
							curConnec->parent->cgi_state = DONE;
							curConnec->parent->pollFd.events = POLLOUT;
							closeConnection();
						}

					}
					break;
			}
		}
		for (size_t i = 0; i < newConns.size(); i++)
			serverConnections.push_back(newConns[i]);
		newConns.clear();
	}
}
