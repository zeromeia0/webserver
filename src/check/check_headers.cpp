#include "_check.hpp"

int checkHeaders(sHeaders *headers) {
	if (!headers)
		return (0);
	if (headers->path.size() >= 3 && (headers->path.find("/../") != std::string::npos || headers->path.substr(0, 3) == "../" || headers->path.substr(headers->path.size() - 3) == "/.."))
		return (delete headers, 0);
	if (headers->version != "HTTP/1.1" && headers->version != "HTTP/1.0")
		return (delete headers, 0);
	std::map<std::string, std::string>::iterator it = headers->headers.find("host");
	if (it == headers->headers.end())
		return (delete headers, 0);
	return (1);
}
