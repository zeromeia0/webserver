#pragma once

#include "main.hpp"
#include <map>

struct reMime {
	std::string	extension;
	std::string	mime;
};

struct reStatusCode {
	int			code;
	std::string	msg;
};

enum eMethod {
	GET,
	POST,
	DELETE,
	HEAD,
};

struct method {
	eMethod		code;
	std::string	txt;
};

struct formUrlEncoded {
	std::map<std::string, std::string> data;
};

struct formData {
	std::string boundaryLimiter;
	std::string name;
	std::string filename;
	std::string data;
};

struct reHeaders {
	eMethod								method;
	std::string							path;
	std::string							version;
	std::map<std::string, std::string>	headers;
};

struct serverRoute {
	serverRoute() : autoindex(false), uploadEnabled(false) {}
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

struct serverConfigs {
	std::vector<int>					listenPorts;
	std::string							host;
	std::string							serverName;
	size_t								clientMaxBodySize;
	std::map<int, std::string>			errorPages;
	std::vector<serverRoute>			router;
	std::vector<std::string>			confFile;
};

#include MAP_HPP
