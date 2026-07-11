#include "../main.hpp"

std::string intToChar( int value ) {
    std::stringstream iss;
    iss << value;
    std::string str;
    iss >> str;
    return (str);
}

char toLower( unsigned char c ) {
    return std::tolower(c);
}

formData parseBoundary(std::string body) {
    std::string rawHeaders = body.substr(0, body.find("\r\n\r\n"));
    std::string rawContent = body.substr(body.find("\r\n\r\n") + 4);
    std::vector<std::string> tokens = tokenizeHttpRequest(rawHeaders);

    formData form;
    for (size_t i = 0; i < tokens.size(); i++) {
            if (i == 0) { form.boundaryLimiter = tokens[i]; continue; }
            if (tokens[i].substr(0, 6) == "name=\"") { form.name = tokens[i].substr(6, tokens[i].substr(6).find("\"")); }
            if (tokens[i].substr(0, 10) == "filename=\"") { form.filename = tokens[i].substr(10, tokens[i].substr(10).find("\"")); }
    }

    return (form);
}

std::string *getFileContent( std::string path ) {

	// if (access(path.c_str(), F_OK) < 0)
	// 	path = ROOT + std::string(F_404);

	std::fstream file(path.c_str());
	if (!file.is_open())
        return (NULL);

    std::string *content = new std::string("");
    char c;
	while(file.get(c))
		*content = *content + c;

	return (content);
};
