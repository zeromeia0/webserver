#include "_parse.hpp"

std::string parseFormData(std::string payload, std::string contentType) {
	if (contentType.empty() || contentType.find("boundary=") == std::string::npos)
		return (payload);
	std::string boundaryLimiter = contentType.substr(contentType.find("boundary=") + 9);
	std::string content;
	size_t start = payload.find("\r\n\r\n");
	size_t end = payload.find("\r\n--" + boundaryLimiter);
	if (start == std::string::npos || end == std::string::npos)
		return (payload);
	start = start + 4;
	content += payload.substr(start, end - start);
	if (end == payload.find("\r\n--" + boundaryLimiter + "--\r\n")) {
		return (content);
	} else {
		content += "\n";
		content += parseFormData(payload.substr(end + 4 + boundaryLimiter.size()), contentType);
	}
	return (content);
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
