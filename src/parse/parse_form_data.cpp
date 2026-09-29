#include "_parse.hpp"

static std::string param(const std::string &hdrs, const std::string &key) {
	size_t p = hdrs.find(key);
	if (p == std::string::npos)
		return ("");
	p += key.size();
	size_t e = hdrs.find('"', p);
	return (e == std::string::npos ? "" : hdrs.substr(p, e - p));
}

std::vector<sFormPart> parseFormData(const std::string &payload, const std::string &contentType) {
	std::vector<sFormPart> parts;
	size_t b = contentType.find("boundary=");
	if (b == std::string::npos)
		return (parts);
	std::string boundary = contentType.substr(b + 9);
	if (boundary.size() >= 2 && boundary[0] == '"')
		boundary = boundary.substr(1, boundary.size() - 2);
	std::string delim = "--" + boundary;
	size_t pos = payload.find(delim);
	while (pos != std::string::npos) {
		pos += delim.size();
		if (payload.compare(pos, 2, "--") == 0)
				break;
		pos += 2;
		size_t hEnd = payload.find("\r\n\r\n", pos);
		if (hEnd == std::string::npos)
			break;
		size_t next = payload.find("\r\n" + delim, hEnd + 4);
		if (next == std::string::npos)
			break;
		std::string hdrs = payload.substr(pos, hEnd - pos);
		sFormPart part;
		part.name = param(hdrs, "; name=\"");
		part.filename = param(hdrs, "filename=\"");
		part.content = payload.substr(hEnd + 4, next - (hEnd + 4));
		parts.push_back(part);
		pos = next + 2;
	}
	return (parts);
}
