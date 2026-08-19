#include "Server.hpp"

void Server::STATUS( int status_code ) {
	curClient->RES->statusCode = status_code;
	std::string content;
	if (serverConfigs->errorPages.find(status_code) != serverConfigs->errorPages.end()) {
		curClient->RES->headers.path = serverConfigs->errorPages[status_code];
		content = readFileContent(curClient->RES->headers.path);
	} else {
		content = intToChar(status_code) + " " + *getStatusMsg(status_code);
		curClient->RES->addHeader("Content-Type", "text/plain");
	}
	curClient->RES->addPayload(content);
};

void Server::OUT() {
	LOG("DEBUG", __FUNCTION__);
	curClient->REQ->saveLog();
	
	Response	*RES = curClient->RES;
	sRoute		ROUT = findRoute(RES->headers.path, serverConfigs->router);
	RE_METHOD	METH = RES->headers.method;
	std::string	PATH = ROUT.root + RES->headers.path;

	bool MethodNotAllowed = !valueInContainer<std::string>(getMethodTxt(METH), ROUT.methods);
	bool PayloadTooLarge = curClient->REQ->payloadLen > serverConfigs->clientMaxBodySize;
	bool MovedPermanently = !ROUT.redirect.empty();
	if (MethodNotAllowed) {
		STATUS(405);
	} else if (PayloadTooLarge) {
		STATUS(413);
	} else if (MovedPermanently) {
		RES->headers.path = ROUT.redirect;
		RES->addHeader("Location", RES->headers.path);
		STATUS(301);
	} else if (ROUT.cgi.find(*getFileExtension(PATH)) != ROUT.cgi.end()) {
		RES->headers.path = PATH;
		if (!(access(RES->headers.path.c_str(), F_OK) == 0)) {
			STATUS(404);
		} else if (!(access(RES->headers.path.c_str(), R_OK) == 0)) {
			STATUS(403);
		} else {
			std::map<std::string, std::string> inputs;
			inputs.insert(std::pair<std::string, std::string>("QUERY_STRING", mapToJsonString<std::string, std::string>(*curClient->REQ->headers.query)));
			std::string *ret = cgi(
				(char *)ROUT.cgi[*getFileExtension(RES->headers.path)].c_str(),
				(char *)RES->headers.path.c_str(),
				inputs,
				curClient->REQ->payload
			);
			if (ret) {
				RES->addPayload(*ret);
				RES->statusCode = 200;
			} else {
				STATUS(500);
			}
		}
	} else {
		switch (METH) {
			case GET:
			case HEAD: {
				DIR *dir = opendir(PATH.c_str());
				int _errno = errno;
				if (dir && !(_errno == EACCES)) {
					closedir(dir);
					if (!ROUT.index.empty()) {
						RES->headers.path = ROUT.root + ROUT.index;
						RES->statusCode = 200;
						RES->addPayload(readFileContent(RES->headers.path));
					} else if (ROUT.autoindex && !(_errno == EACCES)) {
						RES->addPayload(autoindex(PATH, RES->headers.path));
						RES->addHeader("Content-Type", "text/html");
						RES->headers.path = PATH;
						RES->statusCode = 200;
					} else {
						RES->headers.path = PATH;
						STATUS(403);
					}
				} else {
					RES->headers.path = PATH;
					if (!(access(RES->headers.path.c_str(), F_OK) == 0)) {
						STATUS(404);
					} else if (!(access(RES->headers.path.c_str(), R_OK) == 0)) {
						STATUS(403);
					} else {
						RES->addPayload(readFileContent(RES->headers.path));
						RES->statusCode = 200;
					}
				}
				break;
			}
			case POST: {
				RES->headers.path = PATH;
				sFormData *form = parseFormData(curClient->REQ->payload);
				LOG("PAYLOAD", form->payload);
				writeFileContent(RES->headers.path, form->payload) ? STATUS(201) : STATUS(404);
				delete form;
				break;
			}
			case DELETE: {
				LOG("DELETE", PATH);
				std::remove(PATH.c_str());
				RES->statusCode = 200;
				break;
			}
			default: {
				STATUS(500);
				break;
			}
		}
	}

	if (!RES->payload.empty()) {
		std::string *mime = getMimeType(getFileExtension(RES->headers.path));
		if (mime)
			RES->addHeader("Content-Type", *mime);
		RES->addHeader("Content-Length", intToChar(RES->payload.size()));
	}
	if (DEBUG) {
		std::cout << "---------- REQ ----------" << std::endl;
		debugRe(*curClient->REQ, true);
		std::cout << "---------- RES ----------" << std::endl;
		debugRe(*curClient->RES, true);
	}
	RES->saveLog();
	if (METH == HEAD) {
		RES->payload.clear();
		RES->payloadLen = 0;
	}
	LOG("DEBUG", "HELLO");
	RES->stringify();
	LOG("DEBUG", "HELLO");
	send(curConnec->pollFd.fd, RES->body.c_str(), RES->body.size(), 0); // CHECK THE BYTES SENT IF MULTI PACKAGE
}
