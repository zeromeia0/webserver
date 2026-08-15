#pragma once

#include "#MAIN.hpp"
#include EXEC_HPP
#include UTILS_HPP
#include <dirent.h>
#include <unistd.h>

class Re {
public:
	TYPE			type;
	std::string		body;
	sHeaders		headers;
    std::string		payload;
    size_t			payloadLen;

	Re();
	Re( TYPE nType );
	Re( const Re &other );
	Re &operator=( const Re &other );
	~Re();

    const char		*getHeader( std::string strKey );
	void			addHeader( std::string strKey, std::string strValue );
	void			addPayload( const std::string newContent );
	void			saveLog();
};

#include RESPONSE_HPP
#include REQUEST_HPP
