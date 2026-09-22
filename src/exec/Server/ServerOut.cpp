#include "Server.hpp"

void Server::STATUS( int status_code ) {
	LOG("DEBUG", __FUNCTION__ << " " << status_code);
	curClient->RES->statusCode = status_code;
	std::string content;
	if (serverConfigs->errorPages.find(status_code) != serverConfigs->errorPages.end()) {
		curClient->RES->headers.path = serverConfigs->errorPages[status_code];
		content = readFileContent(curClient->RES->headers.path);
	} else {
		content = intToChar(status_code) + " " + getStatusMsg(status_code);
		curClient->RES->addHeader("Content-Type", "text/plain");
	}
	curClient->RES->addPayload(content);
};

void Server::SEND() {
	LOG("DEBUG", __FUNCTION__);
	if (!curClient->RES->payload.empty()) {
		std::string *mime = getMimeType(getFileExtension(curClient->RES->headers.path));
		if (mime) {
			curClient->RES->addHeader("Content-Type", *mime);
		}
		curClient->RES->addHeader("Content-Length", intToChar(curClient->RES->payload.size()));
	}

	if (curClient->RES->headers.method == HEAD) {
		curClient->RES->payload.clear();
	}

	curClient->RES->addHeader("Connection", "close");

	if (DEBUG)
		debugRe(*curClient->RES);

	// curClient->REQ->saveLog();
	// curClient->RES->saveLog();

}

std::string Server::getPath() {
	std::string path;
	if (!curRoute.alias.empty()) {
		std::string rest = curClient->REQ->headers.path.substr(curRoute.path.size());
		if (rest.empty() || rest[0] != '/')
			rest = "/" + rest;
		path = curRoute.alias + rest;
	} else {
		path = curRoute.root + curClient->REQ->headers.path;
	}
	return (path);
}

void Server::OUT() {
	LOG("DEBUG", __FUNCTION__);

	Request		*REQ = curClient->REQ;
	Response	*RES = curClient->RES;

	if (REQ->RequestTimedOut || REQ->BadRequest || REQ->MethodNotAllowed || REQ->PayloadTooLarge || REQ->MovedPermanently) {
		if (REQ->RequestTimedOut) {
			STATUS(408);
		} else if (REQ->BadRequest) {
			STATUS(400);
		} else if (REQ->MethodNotAllowed) {
			STATUS(405);
		} else if (REQ->PayloadTooLarge) {
			STATUS(413);
		} else if (REQ->MovedPermanently) {
			RES->headers.path = curRoute.redirect;
			RES->addHeader("Location", RES->headers.path);
			STATUS(301);	
		}
		return (SEND());
	}

	if (curConnec->cgi_state == DONE) {
		std::string &cgiOutput = RES->payload;
		size_t headerEnd = cgiOutput.find("\r\n\r\n");
		if (headerEnd == std::string::npos)
			headerEnd = cgiOutput.find("\n\n");
		if (headerEnd != std::string::npos) {
			std::string cgiHeaders = cgiOutput.substr(0, headerEnd);
			size_t sep = (cgiOutput[headerEnd] == '\r') ? 4 : 2;
			cgiOutput = cgiOutput.substr(headerEnd + sep);
			// parse Status line from CGI headers
			size_t statusPos = cgiHeaders.find("Status:");
			if (statusPos != std::string::npos) {
				RES->statusCode = strtoul(cgiHeaders.substr(statusPos + 8).c_str(), NULL, 10);
			} else {
				RES->statusCode = 200;
			}
			// parse Content-Type from CGI headers
			size_t ctPos = cgiHeaders.find("Content-Type:");
			if (ctPos != std::string::npos) {
				size_t ctEnd = cgiHeaders.find("\n", ctPos);
				std::string ct = cgiHeaders.substr(ctPos + 14, ctEnd - ctPos - 14);
				// trim trailing \r if present
				if (!ct.empty() && ct[ct.size() - 1] == '\r')
					ct = ct.substr(0, ct.size() - 1);
				RES->addHeader("Content-Type", ct);
			}
		} else {
			RES->statusCode = 200;
		}
		return (SEND());
	}

	std::string PATH = getPath();
	switch (curMethod) {
		case HEAD:
		case GET: {
			RES->headers.path = PATH;	
			DIR *dir = opendir(PATH.c_str());
			int _errno = errno;
			if (dir && !(_errno == EACCES)) {
				closedir(dir);
				if (!curRoute.index.empty()) {
					std::string indexPath = PATH + curRoute.index;
					if (access(indexPath.c_str(), F_OK) == 0) {
						RES->headers.path = indexPath;
						RES->statusCode = 200;
						RES->addPayload(readFileContent(indexPath));
					} else if (curRoute.autoindex) {
						RES->addPayload(autoindex(PATH, curClient->REQ->headers.path));
						RES->addHeader("Content-Type", "text/html");
						RES->headers.path = PATH;
						RES->statusCode = 200;
					} else {
						RES->headers.path = PATH;
						STATUS(404);
					}
				} else if (curRoute.autoindex && !(_errno == EACCES)) {
					RES->addPayload(autoindex(PATH, curClient->REQ->headers.path));
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
			std::string payload = parseFormData(REQ->payload, REQ->headers.get("content-type"));
			if (!payload.empty())
				writeFileContent(RES->headers.path, payload) ? STATUS(201) : STATUS(404);
			else
				STATUS(200);
			break;
		}
		case DELETE: {
			if (access(PATH.c_str(), F_OK) != 0)
				STATUS(404);
			else if (std::remove(PATH.c_str()) != 0)
				STATUS(403);
			else
				STATUS(204);
			break;
		}
		default: {
			STATUS(500);
			break;
		}

	}
}
