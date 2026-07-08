#include "main.hpp"

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
