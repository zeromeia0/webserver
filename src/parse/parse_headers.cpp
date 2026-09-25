#include "_parse.hpp"

sHeaders *parseHeaders( std::string str ) {
	std::vector<std::string> tokens = tokenizeHttpRequest(str);
	sHeaders *headers = new sHeaders;
	int i = 0;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (i == 0) {
			RE_METHOD *method = getMethodCode(*it);
			if (!method) {
				delete headers;
				return (NULL);
			}
			headers->method = *method;
		} else if (i == 1) {
			sFormUrlEncoded form = parseFormUrlEncoded(*it);
			headers->raw = form.raw;
			headers->path = form.path;
			headers->query_str = form.query_str;
		} else if (i == 2) {
			headers->version = *it;
		} else if (*it == "\\r\\n") {
			;
		} else {
			std::pair<std::string, std::string> tmp;
			tmp.first = *it;
			it++;
			if (it == tokens.end())	{delete headers; return (NULL);}
			if (!(*it == ":"))		{delete headers; return (NULL);}
			it++;
			if (it == tokens.end())	{delete headers; return (NULL);}
			while (1) {
				if (*it == ":" || tmp.second.empty() || tmp.second[tmp.second.size() - 1] == ':')
					tmp.second += *it;
				else
					tmp.second += " " + *it;
				if (it + 1 == tokens.end() || *(it + 1) == "\\r\\n")
					break;
				it++; i++;
			}
			std::transform(tmp.first.begin(), tmp.first.end(), tmp.first.begin(), tolower);
			headers->headers.insert(tmp);
		}
		i++;
	}
	return (headers);
};
