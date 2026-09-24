#pragma once

#include "main.hpp"
#include RE_HPP

class Request : public Re {
public:
	std::string		queryString;

	/*Errors occuring during the loop*/
	RUNTIME_ERROR	rError;

	Request();
	Request( const Request &other );
	Request &operator=( const Request &other );
	~Request();
};
