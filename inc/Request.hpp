#pragma once

#include "_main.hpp"
#include RE_HPP

class Request : public Re {
public:
	std::string queryString;

	Request() : Re(REQ) {};
};
