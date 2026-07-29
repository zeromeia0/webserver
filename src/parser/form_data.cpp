#include "../main.hpp"

std::string getFileName( std::string content ) {
    std::vector<std::string> tokens = tokenize(content);
    // printVector(tokens);
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].substr(0, 10) == "filename=\"")
            return (tokens[i].substr(10, tokens[i].substr(10).find("\"")));
    }
    return ("");
}