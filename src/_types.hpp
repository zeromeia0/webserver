#pragma once

#include <iostream>
#include <map>
#include "MAP.hpp"

struct formUrlEncoded {
	std::map<std::string, std::string> data;
};

struct formData {
	std::string boundaryLimiter;
	std::string name;
	std::string filename;
	std::string data;
};

struct HEADERS {
	METHOD								method;
	std::string							path;
	std::string							version;
	std::map<std::string, std::string>	headers;
};

struct ROUTE {
	ROUTE() : autoindex(false), uploadEnabled(false) {}
	std::string							path;
	std::string							root;
	std::string							index;
	std::string							uploadStore;
	std::string							redirect;
	bool								autoindex;
	bool								uploadEnabled;
	std::vector<std::string>			methods;
	std::map<std::string, std::string>	cgi;
};

struct S_CONFIG {
	std::vector<int>					listenPorts;
	std::string							host;
	std::string							serverName;
	size_t								clientMaxBodySize;
	std::map<int, std::string>			errorPages;
	std::vector<ROUTE>					router;
	std::vector<std::string>			confFile;
};
