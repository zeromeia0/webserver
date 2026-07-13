#include "../main.hpp"

void Server::_pollin() {
	LOG("DEBUG", "_pollin");

	if (!sConns[idx].c) {
		createClient();
	} else {
		char str[BUFF_SIZE] = "";
		int bytes = recv(sConns[idx].pfd.fd, str, BUFF_SIZE, 0);
		if (bytes <= 0)
			endConn();
		std::string body(str, bytes);
		sConns[idx].c->receive(body);
		if (sConns[idx].c->getState() == COMPLETED)
			sConns[idx].pfd.events = POLLOUT;
	}
}

void Server::_pollout() {
	LOG("DEBUG", "_pollout");

	routeConfig		route				= findRoute(sConns[idx].c->REQ->getPath());
	Res				r;

	// VARS
	r.vars.method						= sConns[idx].c->REQ->getMethod();
	r.vars.fd							= sConns[idx].pfd.fd;
	r.vars.reqPath						= sConns[idx].c->REQ->getPath();
	r.vars.path							= route.root + r.vars.reqPath;
	r.vars.indexedPath					= route.root + route.index;
	r.vars.dir							= opendir(r.vars.path.c_str());
	r.vars.dir_errno					= errno;

	// CHECKS);
	bool is_method_allowed				= valueInContainer(r.vars.method, route.methods);
	bool is_redirect					= !route.redirect.empty();
	bool is_permission_denied			= (r.vars.dir_errno == EACCES);
	bool is_directory					= ((!r.vars.dir && !is_permission_denied) ? false : true);
	bool is_indexed						= !route.index.empty();
	bool is_autoindex					= route.autoindex;
	bool is_path_accessible				= (access(r.vars.path.c_str(), F_OK) == 0);
	bool is_indexed_path_accessible		= (access(r.vars.indexedPath.c_str(), F_OK) == 0);

	// LOGIC);
	if (!is_method_allowed) {
		return (r.respond(405));										// 405
	}


	if (r.vars.method == "GET") {										// ########### GET
		if (is_redirect)
			return (r.respond(301));									// 301
		if (is_directory) {
			if (is_indexed && is_indexed_path_accessible)
				return (r.setPath(r.vars.indexedPath), r.respond(200));	// 200
			if (!is_autoindex && !is_indexed_path_accessible)
				return (r.respond(404));								// 404	
			if (is_autoindex && !is_permission_denied)
				return (r.respond(200));								// 200
			return (r.respond(403));									// 403
		}
		if (is_path_accessible)
			return (r.setPath(r.vars.path), r.respond(200));			// 200
		return (r.respond(404));										// 404
	}

	if (r.vars.method == "POST") {										// ########### POST
		return (r.respond(200));										// 200
	}

}

void Server::LOOP() {
	LOG("DEBUG", "LOOP");
    while (1) {
        if (_poll()) {
			for (idx = 0; idx < sConns.size(); idx++) {
				sConn *conn = &sConns[idx];
                if (conn->pfd.revents & POLLIN) {
					_pollin();
            	} else if (conn->pfd.revents & POLLOUT) {
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
