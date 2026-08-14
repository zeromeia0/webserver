#pragma once

#include "_main.hpp"
#include RE_HPP
#include MAP_HPP

class Response : public Re {
public:
	int			 						statusCode;

	Response();
	Response( const Re &other );
	void stringify();
};
