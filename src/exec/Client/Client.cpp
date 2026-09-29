#include "Client.hpp"

Client::Client() {
	state = READING_HEADERS;
	REQ = NULL;
	RES = NULL;
}

Client::~Client() {
	_free<Request>(REQ);
	_free<Response>(RES);
};
