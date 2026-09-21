#include "_exec.hpp"

static std::string handleChunk(std::string *bytes) {
	size_t startPos = bytes->find("\r\n");
	if (startPos == std::string::npos)
		return ("");
	size_t endPos = bytes->find("\r\n", startPos + 2);
	if (endPos == std::string::npos)
		return ("");
	std::string hexStr = bytes->substr(0, startPos);
	char *end;
	long chunkSize = strtol(hexStr.c_str(), &end, 16);
	if (end == hexStr.c_str() || *end != '\0' || chunkSize < 0)
		THROW("Invalid chunk size: \"" + std::string(end) + "\"");
	if ((long)(endPos - startPos - 2) < chunkSize)
		THROW("Incomplete chunk data");
	if (chunkSize == 0)
		return ("\1");
	std::string newPayload = bytes->substr(startPos + 2, chunkSize);
	*bytes = bytes->substr(startPos + 2 + chunkSize + 2);
	return (newPayload);
}

std::string parseChunkedBody(std::string *newBytes, std::string *previousBytes, int *status) {
	std::string bytes = *previousBytes + *newBytes;
	previousBytes->clear();
	size_t prevBytesLen = INT_MAX;
	std::string payload;
	while (bytes.size() < prevBytesLen) {
		try {
			prevBytesLen = bytes.size();
			std::string chunk = handleChunk(&bytes);
			if (chunk == std::string("\1"))
				return (*status = 0, payload);
			payload += chunk;
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return (*status = -1, payload);
		}
	}
	if (!bytes.empty())
		*previousBytes = bytes;
	return (*status = 1, payload);
}

// int main() {
// 	std::string remaining = "wevfew7\r\nbon";
// 	std::string payload;
// 	std::string bytes;
// 	int status;

// 	bytes = "jour\r\n6\r\n hello\r\n1\r\n1\r\n5";
// 	payload += handleChunkedBody(&bytes, &remaining, &status);
// 	// std::cerr << "Status = " << status << std::endl;
// 	// std::cerr << "Payload = \"" << payload << "\"" << std::endl;
// 	// std::cerr << "Remaining = \"" << remaining << "\"" << std::endl;

// 	bytes = "\r\nhello\r\n6\r\n hello\r\n1";
// 	payload += handleChunkedBody(&bytes, &remaining, &status);
// 	// std::cerr << "Status = " << status << std::endl;
// 	// std::cerr << "Payload = \"" << payload << "\"" << std::endl;
// 	// std::cerr << "Remaining = \"" << remaining << "\"" << std::endl;

// 	bytes = "\r\n1\r\n0\r\n\r\ndf234fr234";
// 	payload += handleChunkedBody(&bytes, &remaining, &status);
// 	std::cerr << "Status = " << status << std::endl;
// 	std::cerr << "Payload = \"" << payload << "\"" << std::endl;
// 	std::cerr << "Remaining = \"" << remaining << "\"" << std::endl;

// 	if (payload == "bonjour hello1hello hello1") {
// 		std::cerr << "✅ OK" << std::endl;
// 	} else {
// 		std::cerr << "❌ KO" << std::endl;
// 	}

// 	return (0);
// }
