#include "Re.hpp"

Re::Re() : status_code(0), payloadLen(0) {}
Re::Re( const Re &other ) { *this = other; }
Re &Re::operator=( const Re &other ) {
	if (this != &other) {
		this->body = other.body;
		this->headers.version = other.headers.version;
		this->headers.path = other.headers.path;
		this->headers.headers = other.headers.headers;
		this->headers.method = other.headers.method;
		this->status_code = other.status_code;
		this->payload = other.payload;
		this->payloadLen = other.payloadLen;
	}
	return (*this);
};
Re::~Re() {};

const char *Re::getHeader( std::string strKey ) {
	std::map<std::string, std::string>::iterator it = headers.headers.find(strKey);
	if (it == headers.headers.end())
		return (NULL);
	return (it->second.c_str());
}

void Re::addHeader(std::string strKey, std::string strValue) {
	headers.headers.insert(std::pair<std::string, std::string>(strKey, strValue));
};

void Re::addPayload(const std::string newContent) {
	this->payload = this->payload + newContent;
	this->payloadLen = payload.length();
};

void Re::saveLog(std::string type) {
	// LOG("DEBUG", __FUNCTION__);
	char *args[] = {
		(char *)"/usr/bin/python3",
		(char *)"./var/cgi-bin/add_log.py",
		NULL
	};
	std::map<std::string, std::string> map;
	map.insert(std::pair<std::string, std::string>("method", *getMethodTxt(this->headers.method)));
	map.insert(std::pair<std::string, std::string>("path", this->headers.path));
	map.insert(std::pair<std::string, std::string>("type", type));
	if (type == "RES")
		map.insert(std::pair<std::string, std::string>("status", intToChar(this->status_code)));
	else
		map.insert(std::pair<std::string, std::string>("status", ""));
	cgi((const char *)args[0], args, NULL, mapToJsonString(map));
};
