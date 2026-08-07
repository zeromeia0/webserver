#pragma once

#include "main.hpp"
#include RE_HPP
#include MAP_HPP

class Response : public Re {
public:
	Response() {};
	Response( const Re &other ) : Re(other) {};
	void stringify() {
		this->body =
			this->headers.version + " "
			+ intToChar(this->status_code) + " "
			+ *getStatusMsg(this->status_code) + " "
			+ "\r\n";
		for (std::map<std::string, std::string>::iterator it = this->headers.headers.begin(); it != this->headers.headers.end(); ++it)
			this->body = this->body + it->first + ": " + it->second + "\r\n";
		this->body = this->body + "\r\n";
		this->body = this->body + this->payload;
	};	

};
