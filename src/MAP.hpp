#pragma once
#include <iostream>

struct mime;
struct status_code;
enum METHOD {
	GET,
	POST,
	DELETE,
	HEAD,
};
struct method;

std::string *getMethodTxt(METHOD code);
METHOD *getMethodCode(std::string txt);
std::string *getStatusMsg(int status_code);
std::string *getMimeExtension(std::string mime);
std::string *getMimeType(std::string extension);
