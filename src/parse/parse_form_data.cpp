#include "parse.hpp"

std::string getFileName( std::string content ) {
    std::vector<std::string> tokens = tokenize(content);
    // printVector(tokens);
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].substr(0, 10) == "filename=\"")
            return (tokens[i].substr(10, tokens[i].substr(10).find("\"")));
    }
    return ("");
}

formData *parseFormData(std::string body) {
    std::string rawHeaders = body.substr(0, body.find("\r\n\r\n"));
    std::string rawContent = body.substr(body.find("\r\n\r\n") + 4);
    std::vector<std::string> tokens = tokenizeHttpRequest(rawHeaders);

    formData *form = new formData;
    for (size_t i = 0; i < tokens.size(); i++) {
            if (i == 0) { form->boundaryLimiter = tokens[i]; continue; }
            if (tokens[i].substr(0, 6) == "name=\"") { form->name = tokens[i].substr(6, tokens[i].substr(6).find("\"")); }
            if (tokens[i].substr(0, 10) == "filename=\"") { form->filename = tokens[i].substr(10, tokens[i].substr(10).find("\"")); }
    }

    return (form);
}
