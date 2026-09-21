#include "Re.hpp"

Re::Re() {}

Re::Re( RE_TYPE nType ) {
	type = nType;
};

Re::Re( const Re &other ) {
	*this = other;
}

Re &Re::operator=( const Re &other ) {
	if (this != &other) {
		this->type = other.type;
		this->headers.version = other.headers.version;
		this->headers.path = other.headers.path;
		this->headers.query = other.headers.query;
		this->headers.headers = other.headers.headers;
		this->headers.method = other.headers.method;
		this->payload = other.payload;
	}
	return (*this);
};

Re::~Re() {

};

std::string Re::getHeader( std::string strKey ) {
	std::map<std::string, std::string>::iterator it = headers.headers.find(strKey);
	if (it == headers.headers.end())
		return ("");
	return (it->second);
}

void Re::addHeader( std::string strKey, std::string strValue ) {
	headers.headers.insert(std::pair<std::string, std::string>(strKey, strValue));
};

void Re::addPayload( const std::string newContent ) {
	this->payload.append(newContent);
};

void Re::saveLog() {
	// char *bin = (char *)"/usr/bin/python3";
	// char *file = (char *)"./var/cgi-bin/add_log.py";
	std::map<std::string, std::string> map;
	map.insert(std::pair<std::string, std::string>("method", getMethodTxt(this->headers.method)));
	map.insert(std::pair<std::string, std::string>("path", this->headers.path));
	map.insert(std::pair<std::string, std::string>("type", (type == RES ? "RES" : "REQ")));
	if (type == RES)
		map.insert(std::pair<std::string, std::string>("status", intToChar(static_cast<Response*>(this)->statusCode)));
	else
		map.insert(std::pair<std::string, std::string>("status", ""));
	std::map<std::string, std::string> inputs;
	inputs.insert(std::pair<std::string, std::string>("BODY", mapToJsonString<std::string, std::string>(map)));
	std::string payload = "";
	// std::string *output = cgi(bin, file, inputs, payload);
	// delete output;
};

void Re::printRe() {
	std::cerr << getMethodTxt(headers.method) << " ";
	std::cerr << headers.version << " ";
	std::cerr << headers.path << " ";
	std::cerr << payload.size() << " ";
	std::cerr << std::endl;
}
