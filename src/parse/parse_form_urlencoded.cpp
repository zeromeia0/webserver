#include "_parse.hpp"

sFormUrlEncoded parseFormUrlEncoded(std::string url) {
	sFormUrlEncoded f;
	f.raw = url;
	size_t found = url.find("?");
	if (found == std::string::npos) {
		f.path = decodeUrl(url);
		return (f);
	}
	f.path = decodeUrl(url.substr(0, found));
	f.query_str = url.substr(found + 1);
	return (f);
}
