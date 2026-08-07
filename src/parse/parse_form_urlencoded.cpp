#include "parse.hpp"

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

formUrlEncoded *parseFormUrlEncoded(std::string url) {
    std::string raw = url.substr(url.find("?") + 1);
	if (raw.empty())
		return (NULL);
	formUrlEncoded *f = new formUrlEncoded;
	size_t end_pos = 0;
	size_t del_pos = 0;
	while (!raw.empty()) {
		end_pos = raw.find("&");
		if (end_pos == std::string::npos)
			end_pos = raw.size();
		else
			end_pos++;
		del_pos = raw.find("=");
		std::string fst = decode(raw.substr(0, del_pos));
		std::string scd = decode(raw.substr(del_pos + 1, end_pos - (del_pos + 1)));
		f->data.insert(std::pair<std::string, std::string>(fst, scd.substr(0, scd.find("&"))));
		raw = std::string(&raw[end_pos]);
    }
    return (f);
}

// int main() {
// 	std::string str = "vwlerhjvwerlj?bonjour=hello&a=b&b=a&city=new%20york";
// 	formUrlEncoded *form = parseformUrlEncoded(str);
// 	for (std::map<std::string, std::string>::iterator it = form->data.begin(); it != form->data.end(); ++it) {
// 		LOG("NEW STR", it->first << " -> " << it->second);
// 	}
// 	return (0);
// }
