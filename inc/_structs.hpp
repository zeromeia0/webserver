#pragma once

#include "#MAIN.hpp"
#include ENUMS_HPP
#include <map>
#include <vector>
#include <string>

struct sMime {
	std::string							extension;
	std::string							mime;
};

struct sStatusCode {
	int									code;
	std::string							msg;
};

struct sMethod {
	RE_METHOD							code;
	std::string							txt;
};

struct sFormUrlEncoded {
	std::string							path;
	std::map<std::string, std::string>	*query;
	sFormUrlEncoded() : query(NULL) {};
};

struct sFormData {
	std::string 						boundaryLimiter;
	std::string 						name;
	std::string 						filename;
	std::string 						payload;
	sFormData() {};
};

struct sHeaders {
	RE_METHOD							method;
	std::string							path;
	std::map<std::string, std::string>	*query;
	std::string							version;
	std::map<std::string, std::string>	headers;
	TRANSFER_TYPE						transfer_type;
	sHeaders() : query(NULL), transfer_type(CONTENT) {};
};

struct sRoute {
	std::string							path;
	std::string							root;
	std::string							index;
	std::string							uploadStore;
	std::string							redirect;
	std::string							alias;
	bool								autoindex;
	bool								uploadEnabled;
	std::vector<std::string>			methods;
	std::map<std::string, std::string>	cgi;
	sRoute() : autoindex(false), uploadEnabled(false) {};
};

struct sConfigs {
	std::vector<int>					listenPorts;
	std::string							host;
	std::string							serverName;
	size_t								clientMaxBodySize;
	std::map<int, std::string>			errorPages;
	std::vector<sRoute>					router;
	std::vector<std::string>			confFile;
	sConfigs() : clientMaxBodySize(0) {};
};
