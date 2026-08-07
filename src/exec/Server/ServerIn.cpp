#include "Server.hpp"

void Server::IN() {
	LOG("DEBUG", __FUNCTION__);
	char buffer[BUFF_SIZE] = "";
	int bytes = recv(curFd, buffer, BUFF_SIZE, 0);
	if (bytes <= 0)
		return;
	std::string newBody(buffer, bytes);

	/* We add every byte to the REQ body */
	curClient->REQ->body += newBody;

	switch (curClient->state) {
		case COMPLETED:
			break;
		case READING_PAYLOAD:
			curClient->REQ->addPayload(newBody);
			break;
		case READING_HEADERS:
			size_t pattern_pos = curClient->REQ->body.find("\r\n\r\n");
			if (pattern_pos == std::string::npos) {
				return;
			} else {
				curClient->state = READING_PAYLOAD;
				curClient->REQ->headers = parseHeaders(curClient->REQ->body.substr(0, pattern_pos));
				curClient->REQ->addPayload(curClient->REQ->body.substr(pattern_pos + 4));
			}
	}

	const char *header = curClient->REQ->getHeader("content-length");
	size_t content_len = header ? std::atol(header) : 0;
	LOG("HEADER", content_len);
	LOG("PAYLOAD LEN", curClient->REQ->payloadLen);
	if (content_len == curClient->REQ->payloadLen) {
		curClient->state = COMPLETED;
		curConnec->pollFd.events = POLLOUT;
	}
	LOG("STATE", curClient->state);
}
