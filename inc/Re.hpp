#pragma once

#include "_main.hpp"
#include EXEC_HPP
#include UTILS_HPP
#include TYPES_HPP
#include <dirent.h>
#include <unistd.h>

class Re {
public:
	reType								type;
	std::string							body;
	reHeaders							headers;
    std::string							payload;
    size_t								payloadLen;

	Re();
	Re(reType type) : type(type) {};
	Re( const Re &other );
	Re &operator=( const Re &other );
	~Re();
    const char *getHeader( std::string strKey );
	void addHeader(std::string strKey, std::string strValue);
	void addPayload(const std::string newContent);
	void saveLog();

};

#include RESPONSE_HPP
#include REQUEST_HPP
