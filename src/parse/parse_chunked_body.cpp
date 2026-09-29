#include "_exec.hpp"

static int handleChunk(std::string *bytes, std::string *out, size_t maxLeft) {
	size_t startPos = bytes->find("\r\n");
	if (startPos == std::string::npos) {
		if (bytes->size() > 1024)
			THROW("Chunk size line too long");
		return (1);
	}
	std::string hexStr = bytes->substr(0, startPos);
	size_t ext = hexStr.find(';');
	if (ext != std::string::npos)
		hexStr = hexStr.substr(0, ext);
	char *end;
	long chunkSize = strtol(hexStr.c_str(), &end, 16);
	if (end == hexStr.c_str() || *end != '\0' || chunkSize < 0)
		THROW("Invalid chunk size: \"" + hexStr + "\"");
	if ((size_t)chunkSize > maxLeft)
		return (-2);
	size_t dataStart = startPos + 2;
	if (bytes->size() - dataStart < (size_t)chunkSize + 2)
		return (1);
	if (bytes->compare(dataStart + chunkSize, 2, "\r\n") != 0)
		THROW("Missing CRLF after chunk data");
	if (chunkSize == 0)
		return (0);
	*out = bytes->substr(dataStart, chunkSize);
	*bytes = bytes->substr(dataStart + chunkSize + 2);
	return (2);
}

std::string parseChunkedBody(std::string *newBytes, std::string *previousBytes, int *status, size_t maxLeft) {
	std::string bytes = *previousBytes + *newBytes;
	previousBytes->clear();
	std::string payload;
	try {
		while (true) {
			std::string chunk;
			int r = handleChunk(&bytes, &chunk, maxLeft - payload.size());
			if (r == 0)
				return (*status = 0, payload);
			if (r == -2)
				return (*status = -2, payload);
			if (r == 1)
				break;
			payload += chunk;
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (*status = -1, payload);
	}
	*previousBytes = bytes;
	return (*status = 1, payload);
}
