#include "Server.hpp"

void Server::STATUS( int status_code ) {
	LOG("DEBUG", __FUNCTION__ << " " << status_code);
	curClient->RES->statusCode = status_code;
	if (status_code == 204)
		return;
	std::string content;
	if (curConnec->conf->errorPages.find(status_code) != curConnec->conf->errorPages.end()) {
		curClient->RES->headers.path = curConnec->conf->errorPages[status_code];
		content = readFileContent(curClient->RES->headers.path);
	} else {
		content = intToChar(status_code) + " " + getStatusMsg(status_code);
		curClient->RES->addHeader("Content-Type", "text/plain");
	}
	curClient->RES->payload = content;
};

void Server::SEND() {
	LOG("DEBUG", __FUNCTION__);
	if (!curClient->RES->payload.empty()) {
		std::string *mime = getMimeType(getFileExtension(curClient->RES->headers.path));
		if (mime) {
			curClient->RES->addHeader("Content-Type", *mime);
		}

	}

	if (curClient->RES->statusCode != 204)
		curClient->RES->addHeader("Content-Length", intToChar(curClient->RES->payload.size()));

	if (curClient->RES->headers.method == HEAD)
		curClient->RES->payload.clear();

	curClient->RES->addHeader("Connection", "close");

	// curClient->REQ->saveLog();
	// curClient->RES->saveLog();

}

static std::string safeName( std::string name ) {
	size_t s = name.find_last_of("/\\");
	if (s != std::string::npos)
		name = name.substr(s + 1);
	return ((name == "." || name == "..") ? "" : name);
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

	switch (REQ->rError) {
		case (NONE):
			break;
		case (RequestTimedOut):
			return (STATUS(408));
		case (BadRequest):
			return (STATUS(400));
		case (MethodNotAllowed): {
			std::string allow;
			for (size_t i = 0; i < curRoute.methods.size(); i++)
				allow += (i ? ", " : "") + curRoute.methods[i];
			RES->addHeader("Allow", allow);
			return (STATUS(405));
		}
		case (PayloadTooLarge):
			return (STATUS(413));
		case (MovedPermanently):
			RES->headers.path = curRoute.redirect;
			RES->addHeader("Location", RES->headers.path);
			return (STATUS(301));
		case (NotFound):
			return (STATUS(404));
		case (Forbidden):
			return (STATUS(403));
		case (HeaderTooLarge):
			return (STATUS(431));
		case (InternalServerError):
			return (STATUS(500));
		case (NotImplemented):
			return (STATUS(501));
		case (BadGateway):
			return (STATUS(502));
		case (GatewayTimeout):
			return (STATUS(504));
	}

	if (curConnec->cgi_state == DONE) {
		std::string &out = RES->payload;
		size_t end = out.find("\r\n\r\n");
		size_t sep = 4;
		if (end == std::string::npos) {
			end = out.find("\n\n");
			sep = 2;
		}
		if (end == std::string::npos) {
			out.clear();
			return (STATUS(502));
		}
		std::istringstream hs(out.substr(0, end));
		out.erase(0, end + sep);
		RES->statusCode = 200;
		std::string line;
		while (std::getline(hs, line)) {
			if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
			size_t c = line.find(':');
			if (c == std::string::npos)
				continue;
			std::string key = line.substr(0, c);
			std::string val = line.substr(c + 1);
			val.erase(0, val.find_first_not_of(' '));
			std::string low = key;
			std::transform(low.begin(), low.end(), low.begin(), toLower);
			if (low == "status")
				RES->statusCode = atoi(val.c_str());
			else if (low == "content-length")
				continue;
			else if (low == "content-type")
				RES->addHeader("Content-Type", val);
			else
				RES->addHeader(key, val);
		}
		if (RES->getHeader("Content-Type").empty())
			RES->addHeader("Content-Type", "text/html");
		return;
	}

	std::string PATH = getPath();
	switch (curMethod) {
		case HEAD:
		case GET: {
			struct stat st;
			RES->headers.path = PATH;
			if (stat(PATH.c_str(), &st) != 0)
				return (STATUS(404));
			if (S_ISDIR(st.st_mode)) {
				std::string indexPath = PATH;
				if (indexPath.empty() || indexPath[indexPath.size() - 1] != '/')
					indexPath += "/";
				indexPath += (!curRoute.index.empty() && curRoute.index[0] == '/') ? curRoute.index.substr(1) : curRoute.index;
				if (!curRoute.index.empty() && access(indexPath.c_str(), F_OK) == 0) {
					if (access(indexPath.c_str(), R_OK) != 0)
						return (STATUS(403));
					RES->headers.path = indexPath;
					RES->addPayload(readFileContent(indexPath));
				} else if (curRoute.autoindex) {
					RES->addPayload(autoindex(PATH, curClient->REQ->headers.path));
					RES->addHeader("Content-Type", "text/html");
				} else if (!curRoute.index.empty())
					return (STATUS(404));
				else
					return (STATUS(403));
			} else {
				if (access(PATH.c_str(), R_OK) != 0)
					return (STATUS(403));
				RES->addPayload(readFileContent(PATH));
			}
			RES->statusCode = 200;
			break;
		}
		case POST: {
			if (!curRoute.uploadEnabled)
					return (STATUS(403));
			std::string dir = curRoute.root + curRoute.uploadStore;
			std::string ct = REQ->headers.get("content-type");
			if (ct.find("multipart/form-data") != std::string::npos) {
				std::vector<sFormPart> parts = parseFormData(REQ->payload, ct);
				int saved = 0;
				for (size_t i = 0; i < parts.size(); i++) {
					std::string name = safeName(parts[i].filename);
					if (name.empty())
						continue;
					if (!writeFileContent(dir + "/" + name, parts[i].content))
						return (STATUS(500));
					saved++;
				}
				return (saved ? STATUS(201) : STATUS(400));
			}
			std::string target = curRoute.uploadStore.empty() ? PATH : dir + "/" + safeName(REQ->headers.path);
			struct stat st;
			if (stat(target.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
				return (STATUS(400));
			return (writeFileContent(target, REQ->payload) ? STATUS(201) : STATUS(500));
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
