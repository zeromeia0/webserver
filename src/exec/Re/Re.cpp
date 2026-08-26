#include "Re.hpp"

Re::Re() {
	payloadLen = 0;
}

Re::Re( RE_TYPE nType ) {
	type = nType;
};

Re::Re( const Re &other ) {
	*this = other;
}

Re &Re::operator=( const Re &other ) {
	if (this != &other) {
		this->type = other.type;
		this->body = other.body;
		this->headers.version = other.headers.version;
		this->headers.path = other.headers.path;
		this->headers.query = other.headers.query;
		this->headers.headers = other.headers.headers;
		this->headers.method = other.headers.method;
		this->payload = other.payload;
		this->payloadLen = other.payloadLen;
	}
	return (*this);
};

Re::~Re() {

};

const char *Re::getHeader( std::string strKey ) {
	std::map<std::string, std::string>::iterator it = headers.headers.find(strKey);
	if (it == headers.headers.end())
		return (NULL);
	return (it->second.c_str());
}

void Re::addHeader( std::string strKey, std::string strValue ) {
	headers.headers.insert(std::pair<std::string, std::string>(strKey, strValue));
};

void Re::addPayload( const std::string newContent ) {
	if (headers.transfer_type == CHUNKED) {
		size_t pos_start = newContent.find("\r\n") + 2;
		size_t pos_end = newContent.substr(pos_start).find("\r\n");
		this->payload = this->payload + newContent.substr(pos_start, pos_end);
	} else {
		this->payload = this->payload + newContent;
	}
	this->payloadLen = payload.length();
};

void Re::saveLog() {
	char *bin = (char *)"/usr/bin/python3";
	char *file = (char *)"./var/cgi-bin/add_log.py";
	std::map<std::string, std::string> map;
	map.insert(std::pair<std::string, std::string>("method", *getMethodTxt(this->headers.method)));
	map.insert(std::pair<std::string, std::string>("path", this->headers.path));
	map.insert(std::pair<std::string, std::string>("type", (type == RES ? "RES" : "REQ")));
	if (type == RES)
		map.insert(std::pair<std::string, std::string>("status", intToChar(static_cast<Response*>(this)->statusCode)));
	else
		map.insert(std::pair<std::string, std::string>("status", ""));
	std::map<std::string, std::string> inputs;
	inputs.insert(std::pair<std::string, std::string>("BODY", mapToJsonString<std::string, std::string>(map)));
	std::string payload = "";
	cgi(bin, file, inputs, payload);
};

void Re::printRe() {
	std::cout << *getMethodTxt(headers.method) << " ";
	std::cout << headers.version << " ";
	std::cout << headers.path << " ";
	std::cout << payloadLen << " ";
	std::cout << std::endl;
}
