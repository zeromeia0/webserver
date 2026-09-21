#pragma once

#include "#MAIN.hpp"
#include RE_HPP

class Request : public Re {
public:
	std::string		queryString;

	/*Errors occuring during the loop*/
	bool			BadRequest;
	bool			RequestTimedOut;
	bool			MethodNotAllowed;
	bool			PayloadTooLarge;
	bool			MovedPermanently;

	Request();
	Request( const Request &other );
	Request &operator=( const Request &other );
	~Request();
};
