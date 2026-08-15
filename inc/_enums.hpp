#pragma once

#include "#MAIN.hpp"

enum STATE {
	READING_HEADERS,
    READING_PAYLOAD,
    COMPLETED,
};

enum TYPE {
	RES,
	REQ,
};

enum METHOD {
	GET,
	POST,
	DELETE,
	HEAD,
};
