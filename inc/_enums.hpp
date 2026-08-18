#pragma once

#include "#MAIN.hpp"

enum CLIENT_STATE {
	READING_HEADERS,
    READING_PAYLOAD,
    COMPLETED,
};

enum TRANSFER_TYPE {
	CONTENT,
	CHUNKED,
};

enum RE_TYPE {
	RES,
	REQ,
};

enum RE_METHOD {
	GET,
	POST,
	DELETE,
	HEAD,
};
