#include "#MAIN.hpp"
#include SERVER_HPP

int main(int argc, char **argv) {

    if (argc != 2)
    {
        std::cerr << "Error: Invalid Argument" << std::endl;
        std::cout << "Usage: ./webserv config.info" << std::endl;
        return (1);
    }

    try {
        Server S(argv[1]);
        S.START();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return (0);
}
