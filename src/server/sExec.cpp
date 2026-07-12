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
					routeConfig route = findRoute(sConns[idx].c->REQ->getPath());
					int fd = sConns[idx].pfd.fd;

					Res res;
					res.setPath(route.root + sConns[idx].c->REQ->getPath());
					res.setVersion("HTTP/1.1");

					if (!isMethodAllowed(method, route.methods)) {
						res.setStatusCode(405);
						res.respond(fd);
						endConn();
						continue;
					}

					// try {

					// } catch (e) {
					// 	RESPOND 500
					// }
					if (method == "GET") {

						if (!route.redirect.empty()) {
							res.setStatusCode(301);
							res.addHeader("Location", res.getPath());
							res.respond(fd);
							endConn();
							continue;
						}

						bool isDir = false;
						DIR *dir = opendir(res.getPath().c_str());
						if (errno == EACCES) {
							res.setStatusCode(403);
							res.respond(fd);
							endConn();
							continue;
						}
						isDir = (!dir ? false : true);
						LOG("DEBUG", "isDir -> " << isDir);
						LOG("DEBUG", "PATH -> " << res.getPath());

						// THE PATH IS A DIR
						if (isDir) {
							if (!route.index.empty()) {
								res.setPath(route.root + route.index);
								// If the next statement is false,
								// we will use the previous path
								// in the FILE section below
								if (route.autoindex && !(access(res.getPath().c_str(), F_OK) == 0)) {
									res.setStatusCode(200);
									res.makeAutoindexRes(dir, route.path); // Check for errors inside function
									res.respond(fd);
									endConn();
									continue;
								}
							} else if (route.autoindex) {
								res.setStatusCode(200);
								res.makeAutoindexRes(dir, route.path); // Check for errors inside function
								res.respond(fd);
								endConn();
								continue;
							} else {
								res.setStatusCode(403);
								res.respond(fd);
								endConn();
								continue;
							}
						}

						// THE PATH IS A FILE OR INDEXED
						std::string *content = getFileContent(res.getPath());
						if (!content) {
							res.setStatusCode(404);
							delete content;
							content = getFileContent(route.root + "/404.html");
							res.addHeader("Content-Type", "text/html");
							if (!content) {
								res.setStatusCode(500);
								res.addContent("500 - Internal server error");
								res.respond(fd);
								endConn();
								continue;
							}
						} else {
							res.setStatusCode(200);
							res.addHeader("Content-Type", "text/html"); // TO UPDATE
						}
						res.addContent(*content);
						res.respond(fd);
						delete content;
						endConn();
					}

				}

			}
        }

        LOG("DEBUG", "sConns: " << sConns.size());

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

// METHODS
void Server::GET( int fd, routeConfig route, Res &res, std::string path ) {
	LOG("DEBUG", "GET");

	std::string *content = getFileContent(path);

	if (!content) {
		res.setStatusCode(404);
		content = getFileContent(route.root + "/404.html");
	} else {
		res.setStatusCode(200);
	}

	res.addContent(*content);
	res.addHeader("Content-Type", "text/html");
	res.respond(fd);
	delete content;
}

void Server::POST( int fd, routeConfig route, std::string path ) {
	LOG("DEBUG", "POST");
	std::ofstream oss(path.c_str());
	oss << sConns[idx].c->REQ->getContent();
	oss.close();
	Res res;
	std::string _path = route.root + route.uploadPath;
	// LOG("FOUND", path);
	// formData form = parseBoundary(sConns[idx].c->REQ->getContent());

	res.setStatusCode(200);
	res.setVersion("HTTP/1.1");
	res.respond(fd);
}
