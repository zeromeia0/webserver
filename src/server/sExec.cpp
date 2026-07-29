#include "../main.hpp"

static bool receive(Client *client, const std::string &body) {
    if (client->state == R_HEADERS)
		client->buffer += body;

    size_t findRes = client->buffer.find("\r\n\r\n");
    if (findRes == std::string::npos)
        return true;

    if (!client->REQ) {
        client->state = R_CONTENT;
        client->REQ = new Re;
        client->REQ->addHeaders(client->buffer.substr(0, findRes));
        client->REQ->addPayload(client->buffer.substr(findRes + 4));
    } else {
		client->REQ->addPayload(body);
    }

    const char *content_length = client->REQ->getHeader("content-length");
    if ((size_t)(content_length ? std::atoi(content_length) : 0) == client->REQ->payloadLen) {
        client->state = COMPLETED;
        return false;
    }

    return (true);
};

void Server::_pollin() {
	LOG("DEBUG", "_pollin");

	if (!sConns[idx].client) {
		createClient();
	} else {
		char str[BUFF_SIZE] = "";
		int bytes = recv(sConns[idx].poll_fd.fd, str, BUFF_SIZE, 0);
		if (bytes <= 0)
			endConn();
		std::string body(str, bytes);
		receive(sConns[idx].client, body);
		if (sConns[idx].client->state == COMPLETED)
			sConns[idx].poll_fd.events = POLLOUT;
	}
}

void Server::_pollout() {
	LOG("DEBUG", "_pollout");

	routeConfig		route					= findRoute(sConns[idx].client->REQ->path);
	sConns[idx].client->RES = new Re;
	Re *r = sConns[idx].client->RES;

	// VARS
	// int			fd								= sConns[idx].poll_fd.fd;
	std::string	method							= sConns[idx].client->REQ->method;
	std::string	req_path						= sConns[idx].client->REQ->path;
	std::string	req_content						= sConns[idx].client->REQ->payload;
	size_t		req_content_len					= sConns[idx].client->REQ->payload.size();
	std::string	path							= route.root + req_path;
	std::string	indexed_path					= route.root + route.index;
	std::string	redirect_path					= route.redirect;
	DIR			*dir							= opendir(path.c_str());
	int			dir_errno						= errno;

	std::string upload_root						= route.root + route.uploadStore;
	std::string upload_filename					= getFileName(req_content);
	std::string upload_path						= upload_root + "/" + upload_filename;
	
	// CHECKS
	bool is_method_allowed					= valueInContainer(method, route.methods);
	bool is_payload_too_large				= req_content_len > sConf->clientMaxBodySize;
	bool is_script							= (path.find(".py\0") != std::string::npos) || (path.find(".php\0") != std::string::npos);
	// GET
	bool is_redirect						= !route.redirect.empty();
	bool is_permission_denied				= (dir_errno == EACCES);
	bool is_directory						= ((!dir && !is_permission_denied) ? false : true);
	bool is_indexed							= !route.index.empty();
	bool is_autoindex						= route.autoindex;
	bool is_path_accessible					= (access(path.c_str(), F_OK) == 0);
	bool is_indexed_path_accessible			= (access(indexed_path.c_str(), F_OK) == 0);
	// POST
	bool is_upload_root_accessible			= (access(upload_root.c_str(), F_OK) == 0);
	// bool is_upload_path_accessible			= (access(upload_path.c_str(), F_OK) == 0);

	// LOGIC
	if (!is_method_allowed) {
		return (respond(405));												// 405
	}

	if (is_payload_too_large) {
		return (respond(413));												// 413
	}

	if (is_script) {
		std::string route_cgi = route.cgi[getFileExtension(path).c_str()];
		char *args[] = {
			(char *)route_cgi.c_str(),
			(char *)path.c_str(),
			NULL
		};
		r->addPayload(cgi(route_cgi.c_str(), args, NULL));
		return (respond(200));												// 200
	}

	if (method == "GET") {													// # GET
		if (is_redirect) {
			r->is_redirect = true;
			return (r->path = redirect_path, respond(301));					// 301
		}
		else if (is_directory) {
			if (is_indexed && is_indexed_path_accessible)
				return (r->path = indexed_path, respond(200));				// 200
			if (!is_autoindex && !is_indexed_path_accessible)
				return (respond(404));										// 404
			if (is_autoindex && !is_permission_denied) {
				r->is_autoindex = true;
				r->addPayload(autoindex(path, req_path));
				return (respond(200));										// 200
			}
			return (respond(403));											// 403
		}
		if (is_path_accessible) {
			return (r->path = path, respond(200));							// 200
		}
		return (respond(404));												// 404
	}

	if (method == "POST") {													// # POST
		r->path = path;
		if (is_upload_root_accessible) {
			writeFileContent(r->path, req_content);
			return (respond(201));											// 201
		}
		return (respond(404));												// 404
	}

	if (method == "DELETE") {												// # DELETE
		LOG("DELETE", path);
		std::remove(path.c_str());
		return (respond(200));												// 200
	}

}

void Server::respond( int statusCode ) {
	std::cout << "---------- REQUEST ----------" << std::endl;
	sConns[idx].client->REQ->display();

	Re *r = sConns[idx].client->RES;
	r->status_code = statusCode;
	r->method = sConns[idx].client->REQ->method;
	std::string *content = NULL;
	int isHTML = 0;
	if (statusCode == 200) {
		if (r->is_autoindex) {
			isHTML = 1;
		} else if (!r->path.empty()) {
			content = readFileContent(r->path);
			r->addPayload(*content);
			delete content;
		} else {
			r->body = r->body + "200 - OK";
			isHTML = 1;
		}
	} else {
		switch (statusCode) {
			case 201:
				// r->addHeader("Location", r->path);
				r->addPayload("201 Created");
				break;
			case 301:
				r->addHeader("Location", r->path);
				r->addPayload("301 Moved Permanently");
				break;
			case 403:
				if (sConf->errorPages.find(statusCode) != sConf->errorPages.end()) {
					content = readFileContent(sConf->errorPages[statusCode]);
					r->addPayload(*content);
					delete content;
					isHTML = 1;
				} else {
					r->addPayload("403 - Forbidden");
				}
				break;
			case 404:
				if (sConf->errorPages.find(statusCode) != sConf->errorPages.end()) {
					content = readFileContent(sConf->errorPages[statusCode]);
					r->addPayload(*content);
					delete content;
					isHTML = 1;
				} else {
					r->addPayload("404 - Not Found");
				}
				break;
			case 405:
				if (sConf->errorPages.find(statusCode) != sConf->errorPages.end()) {
					content = readFileContent(sConf->errorPages[statusCode]);
					r->addPayload(*content);
					delete content;
					isHTML = 1;
				} else {
					r->addPayload("405 - Method Not Allowed");
				}
				break;
			case 413:
				if (sConf->errorPages.find(statusCode) != sConf->errorPages.end()) {
					content = readFileContent(sConf->errorPages[statusCode]);
					r->addPayload(*content);
					delete content;
					isHTML = 1;
				} else {
					r->addPayload("413 - Payload Too Large");
				}
				break;
			case 500:
				if (sConf->errorPages.find(statusCode) != sConf->errorPages.end()) {
					content = readFileContent(sConf->errorPages[statusCode]);
					r->addPayload(*content);
					delete content;
					isHTML = 1;
				} else {
					r->addPayload("500 - Internal server error");
				}
				break;
			default:
				if (sConf->errorPages.find(statusCode) != sConf->errorPages.end()) {
					content = readFileContent(sConf->errorPages[statusCode]);
					r->addPayload(*content);
					delete content;
					isHTML = 1;
				} else {
					r->addPayload("500 - Internal server error");
				}
				break;
		}
	}
	r->version = HTTP_VERSION;
	if (!r->payload.empty()) {
		if (r->is_redirect || r->method == "POST")
			;
		else if (isHTML)
			r->addHeader("Content-Type", "text/html");
		else {
			std::string ext = getFileExtension(r->path);
			std::string *mime = getMime(ext);
			r->addHeader("Content-Type", *mime);
		}
		r->addHeader("Content-Length", intToChar(r->payload.size()));
	}
	r->stringify();
	std::cout << "---------- RESPONSE ----------" << std::endl;
	r->display();
	LOG("HELLO", r->body.c_str() << " " << r->body.size());
	send(sConns[idx].poll_fd.fd, r->body.c_str(), r->body.size(), 0); // CHECK THE BYTES SENT IF MULTI PACKAGE
};

void Server::LOOP() {
	LOG("DEBUG", "LOOP");
    while (1) {
        if (_poll()) {
			for (idx = 0; idx < sConns.size(); idx++) {
				sConn *conn = &sConns[idx];
                if (conn->poll_fd.revents & POLLIN) {
					_pollin();
            	} else if (conn->poll_fd.revents & POLLOUT) {
					_pollout();
					endConn();
				}
			}
        }
	}
}

routeConfig Server::findRoute(std::string path) {
	routeConfig ret;
	size_t longest = 0;
	for (std::vector<routeConfig>::const_iterator it = sConf->router.begin(); it != sConf->router.end(); ++it ) {
		std::string routerPath = it->path;
		size_t routerPathSize = routerPath.size();
		std::string pathSubstr = path.substr(0, routerPathSize);
		if (pathSubstr == routerPath && routerPathSize > longest)
			ret = *it;
	}
	return (ret);
}
