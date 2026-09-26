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

// int main() {
	
// 	std::string in;
// 	std::string out;

// 	std::cerr << std::endl << "---- Multipart with one file ----" << std::endl;
// 	in =
// 		"--abcdef123456\r\n"
// 		"Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n"
// 		"Content-Type: text/plain\r\n"
// 		"\r\n"
// 		"hello world\r\n"
// 		"--abcdef123456--\r\n";
// 	LOG("OUT", parseFormData(in, "multipart/form-data; boundary=abcdef123456"));

// 	std::cerr << std::endl << "---- Multipart with one file ----" << std::endl;
// 	in =
// 		"--abc123\r\n"
// 		"Content-Disposition: form-data; name=\"file\"; filename=\"hello.txt\"\r\n"
// 		"Content-Type: text/plain\r\n"
// 		"\r\n"
// 		"bonjour\r\n"
// 		"--abc123--\r\n";
// 	LOG("OUT", parseFormData(in, "multipart/form-data; boundary=abc123"));

// 	std::cerr << std::endl << "---- Multipart with multiple fields ----" << std::endl;
// 	in =
// 		"--XYZZY\r\n"
// 		"Content-Disposition: form-data; name=\"username\"\r\n"
// 		"\r\n"
// 		"admin\r\n"
// 		"--XYZZY\r\n"
// 		"Content-Disposition: form-data; name=\"avatar\"; filename=\"pic.png\"\r\n"
// 		"Content-Type: image/png\r\n"
// 		"\r\n"
// 		"<binary data here>\r\n"
// 		"--XYZZY--\r\n";
// 	LOG("OUT", parseFormData(in, "multipart/form-data; boundary=XYZZY"));

// 	std::cerr << std::endl << "---- Plain JSON (no multipart) ----" << std::endl;
// 	in =
// 		"{\"action\":\"do_thing\"}";
// 	LOG("OUT", parseFormData(in, "application/json"));

// 	std::cerr << std::endl << "---- Plain text (no multipart) ----" << std::endl;
// 	in =
// 		"{\"action\":\"do_thing\"}";
// 	LOG("OUT", parseFormData(in, "application/json"));

// 	std::cerr << std::endl << "---- Multipart with one file ----" << std::endl;
// 	in =
// 		"just raw text";
// 	LOG("OUT", parseFormData(in, "text/plain"));

// 	return (0);
// }
