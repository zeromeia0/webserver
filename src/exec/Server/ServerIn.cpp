#include "Server.hpp"

void Server::IN() {
	LOG("DEBUG", __FUNCTION__);
	curConnec->lastActive = time(NULL);
	char buffer[BUFF_SIZE] = "";
	int bytes = recv(curFd, buffer, BUFF_SIZE, 0);
	if (bytes <= 0)
		return;
	std::string newBody(buffer, bytes);

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
				const char *encoding = curClient->REQ->getHeader("transfer-encoding");
				if (encoding && std::string(encoding) == "chunked") {
					curClient->REQ->headers.transfer_type = CHUNKED;
				}
				curClient->REQ->addPayload(curClient->REQ->body.substr(pattern_pos + 4));
			}
	}

	switch (curClient->REQ->headers.transfer_type) {
		case CONTENT: {
			const char *header = curClient->REQ->getHeader("content-length");
			size_t content_len = header ? std::atol(header) : 0;
			if (content_len == curClient->REQ->payloadLen) {
				curClient->state = COMPLETED;
				curConnec->pollFd.events = POLLOUT;
			}		
			break;
		}
		case CHUNKED: {
			if (newBody.find("0\r\n\r\n") != std::string::npos) {
				curClient->state = COMPLETED;
				curConnec->pollFd.events = POLLOUT;
			}
			break;
		}
	}
	LOG("PAYLOAD LEN", curClient->REQ->payloadLen);
	LOG("CLIENT_STATE", curClient->state);
}
