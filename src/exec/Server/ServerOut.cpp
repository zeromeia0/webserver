#include "Server.hpp"

void Server::ERROR( int status_code ) {
	curClient->RES->status_code = status_code;
	std::string content;
	if (serverConfigs->errorPages.find(status_code) != serverConfigs->errorPages.end()) {
		content = readFileContent(serverConfigs->errorPages[status_code]);
	} else {
		content = intToChar(status_code) + " " + *getStatusMsg(status_code);
	}
	curClient->RES->addPayload(content);
};

void Server::OUT() {
	LOG("DEBUG", __FUNCTION__);
	curClient->REQ->saveLog("REQ");

	Response	*RES = curClient->RES;
	serverRoute	ROUT = findRoute(RES->headers.path, serverConfigs->router);
	eMethod		METH = RES->headers.method;
	std::string	PATH = ROUT.root + RES->headers.path;

	bool MethodNotAllowed = !valueInContainer<std::string>(getMethodTxt(METH), ROUT.methods);
	bool PayloadTooLarge = curClient->REQ->payloadLen > serverConfigs->clientMaxBodySize;
	bool MovedPermanently = !ROUT.redirect.empty();
	if (MethodNotAllowed) {
		ERROR(405);
	} else if (PayloadTooLarge) {
		ERROR(413);
	} else if (MovedPermanently) {
		RES->headers.path = ROUT.redirect;
		RES->addHeader("Location", RES->headers.path);
		ERROR(301);
	} else if (ROUT.cgi.find(getFileExtension(PATH)) != ROUT.cgi.end()) {
		RES->headers.path = PATH;
		if (!(access(RES->headers.path.c_str(), F_OK) == 0)) {
			ERROR(404);
		} else if (!(access(RES->headers.path.c_str(), R_OK) == 0)) {
			ERROR(403);
		} else {
			char *args[] = {
				(char *)ROUT.cgi[getFileExtension(PATH)].c_str(),
				(char *)RES->headers.path.c_str(),
				NULL
			};
			char *envp[] = {
				NULL
			};
			std::string *ret = cgi((const char *)args[0], args, envp, curClient->REQ->payload);
			if (ret) {
				RES->addPayload(*ret);
				RES->status_code = 200;
			} else {
				ERROR(500);
			}
		}
	} else {
		switch (METH) {
			case GET: {
				DIR *dir = opendir(PATH.c_str());
				int _errno = errno;
				if (dir && !(_errno == EACCES)) {
					closedir(dir);
					if (!ROUT.index.empty()) {
						RES->headers.path = ROUT.root + ROUT.index;
						RES->status_code = 200;
						RES->addPayload(readFileContent(RES->headers.path));
					} else if (ROUT.autoindex && !(_errno == EACCES)) {
						RES->addPayload(autoindex(PATH, RES->headers.path));
						RES->addHeader("Content-Type", "text/html");
						RES->headers.path = PATH;
						RES->status_code = 200;
					} else {
						RES->headers.path = PATH;
						ERROR(403);
					}
				} else {
					RES->headers.path = PATH;
					if (!(access(RES->headers.path.c_str(), F_OK) == 0)) {
						ERROR(404);
					} else if (!(access(RES->headers.path.c_str(), R_OK) == 0)) {
						ERROR(403);
					} else {
						RES->addPayload(readFileContent(RES->headers.path));
						RES->status_code = 200;
					}
				}
				break;
			}
			case POST: {
			RES->headers.path = PATH;
				writeFileContent(RES->headers.path, curClient->REQ->payload);
				ERROR(201);
				break;
			}
			case DELETE: {
				LOG("DELETE", PATH);
				std::remove(PATH.c_str());
				RES->status_code = 200;
				break;
			}
			case HEAD: {
				break;
			}
			default: {
				ERROR(500);
				break;
			}
		}
	}
	
	RES->headers.version = HTTP_VERSION;
	if (!RES->payload.empty()) {
		std::string *mime = getMimeType(getFileExtension(RES->headers.path));
		if (mime)
			RES->addHeader("Content-Type", *mime);
		RES->addHeader("Content-Length", intToChar(RES->payload.size()));
	}
	RES->stringify();
	if (DEBUG) {
		std::cout << "---------- REQ ----------" << std::endl;
		debugRe(*curClient->REQ, false);
		std::cout << "---------- RES ----------" << std::endl;
		debugRe(*curClient->RES, false);
	}
	RES->saveLog("RES");
	send(curConnec->pollFd.fd, RES->body.c_str(), RES->body.size(), 0); // CHECK THE BYTES SENT IF MULTI PACKAGE
}
