#include "_parse.hpp"

sFormUrlEncoded parseFormUrlEncoded(std::string url) {
	sFormUrlEncoded f;
	size_t found = url.find("?");
	if (found == std::string::npos) {
		f.path = url;
		return (f);
	}
	f.path = decodeUrl(url.substr(0, found));
	f.query_str = url.substr(found + 1);
	return (f);
}

// int main() {
// 	std::string url = "http://localhost:8090/logs.html?method=%5BGET%5D&status=%5B%5D&type=%5B%5D";
// 	LOG("URL", url);
// 	formUrlEncoded *form = parseFormUrlEncoded(url);
// 	LOG("PATH", form->path);
// 	if (form->query) {
// 		for (std::map<std::string, std::string>::iterator it = form->query->begin(); it != form->query->end(); ++it) {
// 			LOG("NEW STR", it->first << " -> " << it->second);
// 		}
// 	}
// 	return (0);
// }
