#pragma once

#include "main.hpp"
#include ENUMS_HPP
#include <map>
#include <vector>
#include <string>
#include <sys/types.h>

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
	std::string							raw;
	std::string							path;
	std::string							query_str;
	sFormUrlEncoded() {};
};

struct sHeaders {
	RE_METHOD							method;
	std::string							raw;
	std::string							path;
	std::string							script;
	std::string							info;
	std::string							query_str;
	std::map<std::string, std::string>	query;
	std::string							version;
	std::map<std::string, std::string>	headers;
	sHeaders() {};
	std::string get(std::string strKey) {
		std::map<std::string, std::string>::iterator it = this->headers.find(strKey);
		if (it == this->headers.end())
			return ("");
		return (it->second);
	};
};

struct sRoute {
	std::string							path;
	std::string							root;
	std::string							index;
	std::string							uploadStore;
	std::string							redirect;
	std::string							alias;
	ssize_t								clientMaxBodySize;
	bool								autoindex;
	bool								uploadEnabled;
	std::vector<std::string>			methods;
	std::map<std::string, std::string>	cgi;
	sRoute() : clientMaxBodySize(-1), autoindex(false), uploadEnabled(false) {};
};

struct sConfigs {
	std::vector<int>					listenPorts;
	std::string							host;
	std::string							serverName;
	ssize_t								clientMaxBodySize;
	std::map<int, std::string>			errorPages;
	std::vector<sRoute>					router;
	std::vector<std::string>			confFile;
	sConfigs() : clientMaxBodySize(-1) {};
};

struct sCgiState {
	pid_t								pid;
	size_t								written;
	std::string							payload;
	std::string							output;
};
