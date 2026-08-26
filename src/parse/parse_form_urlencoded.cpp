#include "_parse.hpp"

static std::string decode(std::string str) {
	std::string new_str;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '%' && i + 2 < str.size()) {
			std::istringstream iss(str.substr(i + 1, 2));
			int c;
			iss >> std::hex >> c;
			new_str += static_cast<char>(c);
			i += 2;
		} else if (str[i] == '+') {
			new_str += ' ';
		} else {
			new_str += str[i];
		}
	}
	return new_str;
}

sFormUrlEncoded *parseFormUrlEncoded(std::string url) {
	sFormUrlEncoded *f = new sFormUrlEncoded;
	f->path = url.substr(0, url.find("?"));
	f->query = new std::map<std::string, std::string>;
	size_t find = url.find("?");
	if (find == std::string::npos) {
		return (f);
	}
	std::string query_str = url.substr(find + 1);
	size_t end_pos = 0;
	size_t del_pos = 0;
	while (!query_str.empty()) {
		end_pos = query_str.find("&");
		if (end_pos == std::string::npos)
			end_pos = query_str.size();
		else
			end_pos++;
		del_pos = query_str.find("=");
		std::string fst = decode(query_str.substr(0, del_pos));
		std::string scd = decode(query_str.substr(del_pos + 1, end_pos - (del_pos + 1)));
		f->query->insert(std::pair<std::string, std::string>(fst, scd.substr(0, scd.find("&"))));
		query_str = std::string(&query_str[end_pos]);
	}
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
