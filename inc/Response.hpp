#pragma once

#include "main.hpp"
#include RE_HPP
#include MAP_HPP

class Request;

class Response : public Re {
public:
	std::string		body;
	int				statusCode;

	Response();
	Response( const Response &other );
	Response &operator=( const Response &other );
	~Response();

	std::string		stringify();
};
