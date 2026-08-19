#include "_parse.hpp"

std::string getFileName( std::string content ) {
    std::vector<std::string> tokens = tokenize(content);
    // printVector(tokens);
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].substr(0, 10) == "filename=\"")
            return (tokens[i].substr(10, tokens[i].substr(10).find("\"")));
    }
    return ("");
}

sFormData *parseFormData(std::string body) {
    std::string rawHeaders = body.substr(0, body.find("\r\n\r\n"));
    std::vector<std::string> tokens = tokenizeHttpRequest(rawHeaders);

    sFormData *form = new sFormData;
    for (size_t i = 0; i < tokens.size(); i++) {
            if (i == 0) {
				if (tokens[i].find("------WebKitFormBoundary") == std::string::npos) {
					form->payload = body;
					return (form);
				}
				form->boundaryLimiter = tokens[i];
				continue;
			}
            if (tokens[i].substr(0, 6) == "name=\"") { form->name = tokens[i].substr(6, tokens[i].substr(6).find("\"")); }
            if (tokens[i].substr(0, 10) == "filename=\"") { form->filename = tokens[i].substr(10, tokens[i].substr(10).find("\"")); }
    }
	size_t start = body.find("\r\n\r\n") + 4;
	size_t end = body.find("\r\n" + form->boundaryLimiter + "--\r\n");
	form->payload = body.substr(start, end - start);
    return (form);
}

// int main() {
// 	std::string payload =
// 		"------WebKitFormBoundaryiKFQwgjcJQvQrTa9\r\n"
// 		"Content-Disposition: form-data; name=\"file\"; filename=\"hello\"\r\n"
// 		"Content-Type: application/octet-stream\r\n"
// 		"\r\n"
// 		"bonjour\r\n"
// 		"------WebKitFormBoundaryiKFQwgjcJQvQrTa9--\r\n";
// 	sFormData *form = parseFormData(payload);
// 	LOG("boundaryLimiter", form->boundaryLimiter);
// 	LOG("payload", form->payload);
// 	LOG("filename", form->filename);
// 	LOG("name", form->name);
// 	return (0);
// }
