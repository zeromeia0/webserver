#pragma once

#include "#MAIN.hpp"
#include RE_HPP
#include MAP_HPP

class Request;

class Response : public Re {
public:
	int		statusCode;

	Response();
	Response( const Request *req );
	Response( const Response &other );
	Response &operator=( const Response &other );
	~Response();

	void	stringify();
};
