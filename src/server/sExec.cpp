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

					Res res;
					res.setPath(route.root + sConns[idx].c->REQ->getPath());
					res.setVersion("HTTP/1.1");

					bool method_allowed = false;
					for (size_t i = 0; i < route.methods.size(); i++) {
						if (route.methods[i] == method) {
							method_allowed = true;
							break;
						}
					}

					if (!method_allowed) {
						res.setStatusCode(405);
						res.respond(sConns[idx].pfd.fd);
						endConn();
						continue;
					}

					if (!route.redirect.empty()) {
						res.setStatusCode(301);
						res.addHeader("Location", res.getPath());
						res.respond(sConns[idx].pfd.fd);
						endConn();
						continue;
					}

					DIR *dir = opendir(res.getPath().c_str());
					bool isDir = (!dir ? false : true);

					if (isDir) {
						if (!route.index.empty()) {
							res.setPath(route.root + route.index);
							// Will go to GET later on vvv
						}
						if (route.autoindex) {
							res.setStatusCode(200);
							dirent *_dirent = readdir(dir);
							res.addContent("<body>\r\n");
							while ((_dirent = readdir(dir))) {
								std::string cont = std::string(_dirent->d_name);
								res.addContent("<div>");
								res.addContent("<a href=" + cont + ">");
								res.addContent(cont);
								res.addContent("</a>");
								res.addContent("</div>");
							}
							res.addContent("</body>\r\n");
							res.respond(sConns[idx].pfd.fd);
							endConn();
							continue;
						}
					}
					
					if (method == "GET") {
						GET(sConns[idx].pfd.fd, route, res, res.getPath());
					} else if (method == "POST") {
						POST(sConns[idx].pfd.fd, route, res.getPath());
					}

					endConn();
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
