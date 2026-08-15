#include "_debug.hpp"

void debugRe(Re &re, bool printPayload) {
	std::cout << "version: " << re.headers.version << std::endl;
	std::cout << "path: " << re.headers.path << std::endl;
	if (re.headers.query) {
		std::cout << "query: " << std::endl;
		for (std::map<std::string, std::string>::iterator it = re.headers.query->begin(); it != re.headers.query->end(); ++it) {
			std::cout << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
		}
	}
	std::cout << "headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = re.headers.headers.begin(); it != re.headers.headers.end(); ++it) {
		std::cout << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
	}
	std::cout << "method: " << *getMethodTxt(re.headers.method) << std::endl;
	if (re.type == RES) {
		debugRes(static_cast<Response&>(re));
	}
	if (re.type == REQ) {
		debugReq(static_cast<Request&>(re));
	}
	if (printPayload)
		std::cout << "payload: " << re.payload << std::endl;
}

void debugRes(Response &res) {
	std::cout << "status_code: " << res.statusCode << std::endl;
}

void debugReq(Request &req) {
	std::cout << "query_string: " << req.queryString << std::endl;
}
