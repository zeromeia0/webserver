#include "parse.hpp"

reHeaders parseHeaders( std::string str ) {
	std::vector<std::string> tokens = tokenizeHttpRequest(str);
	reHeaders headers;
	int i = 0;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
		if (i == 0)                 { headers.method = *getMethodCode(*it); }
		else if (i == 1)            { headers.path = *it; }
		else if (i == 2)            { headers.version = *it; }
		else if (*it == "\\r\\n")   { ; }
		else {
			std::pair<std::string, std::string> tmp;
			tmp.first = *it;
			it++;
			if (!(*it == ":"))
				break;
			it++;
			while (1) {
				tmp.second += *it;
				if (it + 1 == tokens.end() || *(it + 1) == "\\r\\n")
					break;
				it++; i++;
				tmp.second += " ";
			}
			std::transform(tmp.first.begin(), tmp.first.end(), tmp.first.begin(), tolower);
			headers.headers.insert(tmp);
		}
		i++;
	}
	return (headers);
};
