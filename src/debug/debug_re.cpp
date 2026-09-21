#include "_debug.hpp"

void debugRe(Re &re) {
	std::cerr << "---------- " + std::string(re.type == 0 ? "REQ" : "RES") + " ----------" << std::endl;
	std::cerr << "version: " << re.headers.version << std::endl;
	std::cerr << "path: " << re.headers.path << std::endl;
	if (re.headers.query.size() > 0) {
		std::cerr << "query: " << std::endl;
		for (std::map<std::string, std::string>::iterator it = re.headers.query.begin(); it != re.headers.query.end(); ++it) {
			std::cerr << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
		}
	}

	std::cerr << "headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = re.headers.headers.begin(); it != re.headers.headers.end(); ++it) {
		std::cerr << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
	}
	std::cerr << "method: " << getMethodTxt(re.headers.method) << std::endl;

	switch (re.type) {
		case REQ:
			debugReq(static_cast<Request&>(re));
			break;
		case RES:
			debugRes(static_cast<Response&>(re));
			break;
	}

	std::cerr << "payload: \"" << ((re.payload.size() > 1000) ? (re.payload.substr(0, 1000) + "...") : (re.payload)) << "\"" << std::endl;
	// std::cerr << "payload: \"" << re.payload << "\"" << std::endl;
}

void debugReq(Request &req) {
	std::cerr << "query_string: " << req.queryString << std::endl;
}

void debugRes(Response &res) {
	std::cerr << "status_code: " << res.statusCode << std::endl;
}

