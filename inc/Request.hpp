#pragma once

#include "#MAIN.hpp"
#include RE_HPP

class Request : public Re {
public:
	std::string		queryString;

	Request();
	Request( const Request &other );
	Request &operator=( const Request &other );
	~Request();
};
