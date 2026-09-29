#include "_parse.hpp"

static std::string trimSpaces( const std::string &s ) {
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	size_t end = s.find_last_not_of(" \t");
	return (s.substr(start, end - start + 1));
}

sHeaders *parseHeaders( std::string str ) {
	std::istringstream ss(str);
	std::string line;
	if (!std::getline(ss, line))
		return (NULL);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);

	std::istringstream requestLine(line);
	std::string method, target, version, extra;
	if (!(requestLine >> method >> target >> version) || (requestLine >> extra))
		return (NULL);

	sHeaders *headers = new sHeaders;
	RE_METHOD *code = getMethodCode(method);
	headers->method = code ? *code : UNKNOWN;
	sFormUrlEncoded form = parseFormUrlEncoded(target);
	headers->raw = form.raw;
	headers->path = form.path;
	headers->query_str = form.query_str;
	headers->version = version;

	while (std::getline(ss, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			continue;
		size_t colon = line.find(':');
		if (colon == std::string::npos || colon == 0)
			{delete headers; return (NULL);}
		std::string key = line.substr(0, colon);
		if (key.find_first_of(" \t") != std::string::npos)
			{delete headers; return (NULL);}
		std::transform(key.begin(), key.end(), key.begin(), tolower);
		headers->headers.insert(std::make_pair(key, trimSpaces(line.substr(colon + 1))));
	}
	return (headers);
};
