#include "Server.hpp"

void Server::addConnection() {
	LOG("DEBUG", __FUNCTION__);

}

void Server::closeConnection() {
	LOG("DEBUG", __FUNCTION__ << " " << curFd);
	if (curConnec->type == CLIENT) {
		killCgi(curConnec->cgi_pid);
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
	int ready = poll(&tmp[0], tmp.size(), 1000);
	for (size_t i = 0; i < serverConnections.size(); i++)
		serverConnections[i]->pollFd.revents = (ready > 0) ? tmp[i].revents : 0;
}

bool Server::isCgi() {
	LOG("DEBUG", __FUNCTION__);
	std::string PATH = getPath();
	std::string fileExtension = getFileExtension(PATH);
	return ((curRoute.cgi.find(fileExtension) != curRoute.cgi.end()));
}

std::map<std::string, std::string> Server::handleEnvp(const std::string &scriptFile) {
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

	inputs["SERVER_NAME"]			= curConnec->conf->serverName;
	inputs["SERVER_PORT"]			= intToChar(curConnec->port);
	inputs["GATEWAY_INTERFACE"]		= "CGI/1.1";
	inputs["SERVER_SOFTWARE"]		= "webserv/1.0";
	inputs["REDIRECT_STATUS"]		= "200";
	inputs["SCRIPT_FILENAME"]		= scriptFile;


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
	// if (DEBUG) {
	// 	std::cerr << "---------- CGI ENVP ----------" << std::endl;
	// 	debugMap<std::string, std::string>(inputs);
	// }
	return (inputs);
}

static std::string relativeToDir( const std::string &path, const std::string &dir ) {
	if (path.empty() || path[0] == '/')
		return (path);
	std::string up;
	std::stringstream ss(dir);
	std::string seg;
	while (std::getline(ss, seg, '/'))
		if (!seg.empty() && seg != ".")
			up += "../";
	return (up + path);
}

int Server::startCgi() {
	LOG("DEBUG", __FUNCTION__);

	std::string PATH = getPath();
	if (access(PATH.c_str(), F_OK) == 0 && access(PATH.c_str(), R_OK) != 0)
		return (curClient->REQ->rError = Forbidden, -1);
	std::string cgiPath = curRoute.cgi.find(getFileExtension(PATH))->second;
	
	int pipe_in[2], pipe_out[2];
	if (pipe(pipe_in) < 0)
		return (curClient->REQ->rError = InternalServerError, -1);
	if (pipe(pipe_out) < 0) {
		close(pipe_in[0]); close(pipe_in[1]);
		return (curClient->REQ->rError = InternalServerError, -1);
	}
	fcntl(pipe_in[1], F_SETFD, FD_CLOEXEC);
	fcntl(pipe_out[0], F_SETFD, FD_CLOEXEC);

	pid_t pid = fork();
	if (pid < 0) {
		close(pipe_in[0]); close(pipe_in[1]);
		close(pipe_out[0]); close(pipe_out[1]);
		return (curClient->REQ->rError = InternalServerError, -1);
	}

	if (pid == 0) {
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_in[0]); close(pipe_in[1]);
		close(pipe_out[0]); close(pipe_out[1]);

		size_t slash = PATH.find_last_of('/');
		std::string dir = PATH.substr(0, slash);
		std::string file = PATH.substr(slash + 1);
		std::string interp = relativeToDir(cgiPath, dir);

		if (chdir(dir.c_str()) < 0)
			_exit(1);

		char *args[] = { (char *)interp.c_str(), (char *)file.c_str(), NULL };
		std::map<std::string, std::string> mapEnvp = handleEnvp(file);
		std::vector<std::string> env_strs;
		for (std::map<std::string, std::string>::iterator it = mapEnvp.begin(); it != mapEnvp.end(); ++it)
			env_strs.push_back(it->first + "=" + it->second);

		char **envp = new char*[env_strs.size() + 1];
		for (size_t i = 0; i < env_strs.size(); i++)
			envp[i] = (char *)env_strs[i].c_str();
		envp[env_strs.size()] = NULL;

		execve(interp.c_str(), args, envp);
		_exit(1);
	}
	close(pipe_in[0]);
	close(pipe_out[1]);
	cgiPids.push_back(pid);
	curConnec->cgi_pid = pid;

	Connection *cin = new Connection(-1, pipe_in[1], CGI_IN, curConnec);
	newConns.push_back(cin);

	Connection *cout = new Connection(-1, pipe_out[0], CGI_OUT, curConnec);
	newConns.push_back(cout);

	return (1);
}

bool Server::createNewClient() {
	LOG("DEBUG", __FUNCTION__);
	int clientFd = accept(curFd, NULL, NULL);
	if (clientFd < 0)
		return (false);
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	fcntl(clientFd, F_SETFD, FD_CLOEXEC);
	Connection *newClient = new Connection(curConnec->port, clientFd, CLIENT, NULL);
	newClient->conf = curConnec->conf;
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
		if (curConnec->cgi_state == ONGOING)
			return (1);
		OUT();
		SEND();
		curClient->RES->body = curClient->RES->stringify();
		if (DEBUG)
			debugRe(*curClient->RES);
		else
			LOG("🔴 RES", curClient->RES->statusCode << " size: " << curClient->RES->payload.size() << "\n");
		std::string().swap(curClient->RES->payload);
	}
	Response *RES = curClient->RES;
	int nbytes = send(curFd, RES->body.data() + RES->sent, RES->body.size() - RES->sent, 0);
	if (nbytes <= 0)
			return (0);
	RES->sent += nbytes;
	if (RES->sent == RES->body.size())
			return (0);
	return (1);
}

static int writeToPipe( int fd, const char *payload, size_t remaining ) {
	LOG("DEBUG", __FUNCTION__ << " -> Remaining: " << remaining);
	int nbytes = write(fd, payload, remaining);
	return (nbytes);
}

static int readFromPipe( int fd, std::string *payload ) {
	LOG("DEBUG", __FUNCTION__);
	char buff[BUFF_SIZE];
	int nbytes = read(fd, buff, BUFF_SIZE);
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
	bool waitingOnCgi = (curConnec->type == CLIENT && curConnec->cgi_state == ONGOING);
	if (!waitingOnCgi && (now - curConnec->lastActive) > TIMEOUT)
		return (REQ->rError = RequestTimedOut, -1);
	if (CLI->state == READING_HEADERS)
		return (1);
	if (REQ->payload.size() > (size_t)curRoute.clientMaxBodySize)
		return (REQ->rError = PayloadTooLarge, -3);
	return (1);
}

RUNTIME_ERROR Server::validateRequest() {
	LOG("DEBUG", __FUNCTION__);
	Request *REQ = curClient->REQ;
	if (REQ->headers.method == UNKNOWN)
		return (NotImplemented);
	if (!valueInContainer<std::string>(getMethodTxt(REQ->headers.method), curRoute.methods))
		return (MethodNotAllowed);
	if (!curRoute.redirect.empty())
		return (MovedPermanently);
	std::string cl = REQ->getHeader("content-length");
	if (!cl.empty() && strtoul(cl.c_str(), NULL, 10) > (size_t)curRoute.clientMaxBodySize)
		return (PayloadTooLarge);
	return (NONE);
  }

void Server::finishRequest() {
	LOG("DEBUG", __FUNCTION__);
	Request *REQ = curClient->REQ;
	curClient->state = COMPLETED;
	curConnec->pollFd.events = POLLOUT;
	if (REQ->rError == NONE && REQ->payload.size() > (size_t)curRoute.clientMaxBodySize)
		REQ->rError = PayloadTooLarge;
	if (REQ->rError != NONE || !isCgi())
		return;
	std::string uri = REQ->headers.raw;
	std::string file_ext = getFileExtension(uri);
	size_t ext_pos = uri.find(file_ext);
	REQ->headers.script = uri.substr(0, ext_pos + file_ext.size());
	REQ->headers.info = uri.substr(REQ->headers.script.size());
	if (REQ->headers.info.empty())
		REQ->headers.info = REQ->headers.script;
	if (startCgi() > 0) {
		curConnec->cgi_state = ONGOING;
		curConnec->pollFd.events = 0;
	}
}

void Server::killCgi( pid_t pid ) {
	LOG("DEBUG", __FUNCTION__);
	if (pid > 0 && std::find(cgiPids.begin(), cgiPids.end(), pid) != cgiPids.end())
		kill(pid, SIGKILL);
}

void Server::removeZombiesCgi() {
	LOG("DEBUG", __FUNCTION__);
	for (size_t i = 0; i < cgiPids.size(); ) {
		int status;
		pid_t r = waitpid(cgiPids[i], &status, WNOHANG);
		if (r > 0)
			cgiPids.erase(cgiPids.begin() + i);
		else
			i++;
	}
}

void Server::LOOP() {
	LOG("DEBUG", __FUNCTION__);

	while (G_RUNNING) {
		curConnections = serverConnections.size();

		POLL();
		removeZombiesCgi();

		for (curIdx = 0; curIdx < serverConnections.size(); curIdx++) {

			curConnec		= serverConnections[curIdx];
			curClient		= curConnec->client;
			curPollFd		= curConnec->pollFd;
			curFd			= curPollFd.fd;
			curContentLen	= curConnec->contentLen;
			curRoute = curConnec->parent ? curConnec->parent->route : curConnec->route;

			int connStatus = connectionCheck();
			if (connStatus < 0) {
				if (curConnec->type == CGI_IN || curConnec->type == CGI_OUT) {
					Connection *cli = curConnec->parent;
					killCgi(cli->cgi_pid);
					cli->client->REQ->rError = GatewayTimeout;
					cli->cgi_state = DONE;
					cli->pollFd.events = POLLOUT;
					cli->lastActive = time(NULL);
					closeConnection(); continue;
				} else if (curConnec->type == CLIENT) {
					killCgi(curConnec->cgi_pid);
					curConnec->pollFd.events = POLLOUT;
					curConnec->cgi_state = DEFAULT;
					curClient->RES->payload.clear();
				}
			}

			char buff[BUFF_SIZE];
			switch (curConnec->type) {
				case SERVER:
					if (curConnec->pollFd.revents & POLLIN)
						createNewClient();
					break;
				case CLIENT:
					if (curPollFd.revents & (POLLERR | POLLHUP)) {
						closeConnection();
						break;
					}
					if (curPollFd.revents & POLLIN) {
						curConnec->updateLastActive();
						int nbytes = recv(curFd, buff, BUFF_SIZE, 0);
						if (nbytes <= 0) {
							closeConnection();
							break;
						}
						if (curClient->state == READING_HEADERS) {
							curConnec->buffer.append(buff, nbytes);
							size_t headersEof = curConnec->buffer.find("\r\n\r\n");
							if (headersEof != std::string::npos) {
								if(!handleHeaders(curConnec->buffer.substr(0, headersEof), curConnec)) {
									curConnec->client->REQ->rError = BadRequest;
									if (curConnec->cgi_state == ONGOING)
										curConnec->cgi_state = DONE;
									curConnec->pollFd.events = POLLOUT;
									break;
								}
								if (DEBUG)
									debugRe(*curClient->REQ);
								else
									LOG("🟢 REQ", getMethodTxt(curClient->REQ->headers.method) << " " << curClient->REQ->headers.path);
								curConnec->buffer = curConnec->buffer.substr(headersEof + 4);
								nbytes = 0;
								buff[0] = '\0';
								// postHeadersUpdates
								curClient->RES->headers.method = curClient->REQ->headers.method;
								curClient->RES->headers.version = curClient->REQ->headers.version;
								curClient->RES->headers.path = curClient->REQ->headers.path;
								curMethod = curClient->REQ->headers.method;
								curConnec->route = findRoute(curConnec->client->REQ->headers.path, curConnec->conf->router);
								curRoute = curConnec->route;
								RUNTIME_ERROR err = validateRequest();
								if (err != NONE) {
									curClient->REQ->rError = err;
									finishRequest();
									break;
								}
								curClient->state = READING_PAYLOAD;
								if (curClient->REQ->getHeader("transfer-encoding") == "chunked")
									curConnec->transfer_type = CHUNKED;
								curConnec->contentLen = strtoul(curClient->REQ->getHeader("content-length").c_str(), NULL, 10);
								curContentLen = curConnec->contentLen;
								if (!curContentLen && curConnec->transfer_type == CONTENT) {
									finishRequest();
									break;
								}
							}
						}

						if (curClient->state == READING_PAYLOAD) {
							int status = 0;
							std::string new_payload;
							std::string new_bytes(buff, nbytes);
							if (curConnec->transfer_type == CONTENT) {
								new_payload = curConnec->buffer + std::string(buff, nbytes);
								curConnec->buffer.clear();
								if (curClient->REQ->payload.size() + new_payload.size() < curContentLen)
									status = 1;
							} else if (curConnec->transfer_type == CHUNKED)
								new_payload = parseChunkedBody(&new_bytes, &curConnec->buffer, &status);
							curClient->REQ->payload.append(new_payload);
							switch (status) {
								case 0:		finishRequest(); break;
								case -1:	curClient->REQ->rError = BadRequest; finishRequest(); break;
								case 1:		break;
							}
						}
					} else if (curConnec->pollFd.revents & POLLOUT) {
						if (curConnec->cgi_state == ONGOING)
							break;
						if (sendDataToClient() == 0)
							closeConnection();
					}
					break;
				case CGI_IN:
					if (curConnec->pollFd.revents & POLLOUT) {
						const char *payload = curConnec->parent->client->REQ->payload.data() + curConnec->parent->cgi_offset;
						size_t remaining = curConnec->parent->client->REQ->payload.size() - curConnec->parent->cgi_offset;
						if (!remaining) {
							std::string().swap(curConnec->parent->client->REQ->payload);
							closeConnection();
							break;
						}
						int nbytes = writeToPipe(curFd, payload, remaining);
						if (nbytes < 0) {
							curConnec->parent->client->REQ->rError = BadGateway;
							curConnec->parent->cgi_state = DONE;
							curConnec->parent->pollFd.events = POLLOUT;
							closeConnection();
							break;
						}
						if (nbytes > 0)
							curConnec->updateLastActive();
						curConnec->parent->cgi_offset += nbytes;
					}
					break;
				case CGI_OUT:
					if (curConnec->pollFd.revents & (POLLIN | POLLHUP)) {
						curConnec->updateLastActive();
						int nbytes = readFromPipe(curConnec->pollFd.fd, &curConnec->parent->client->RES->payload);
						if (nbytes < 0) {
							curConnec->parent->client->REQ->rError = BadGateway;
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
