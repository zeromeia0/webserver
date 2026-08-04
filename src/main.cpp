#include "_main.hpp"
#include "_types.hpp"
#include "exec/Server/Server.hpp"
#include "exec/_utils.hpp"
#include "parser/_utils.hpp"

// MODE mode = DEV;
// MODE mode = PROD;

int main(int argc, char **argv) {

    // CHECK INPUTS
    if (argc != 2)
    {
        std::cerr << "Error: Invalid Argument" << std::endl;
        std::cout << "Usage: ./webserv config.info" << std::endl;
        return (1);
    }

    // START SERVER
    try {
        Server S(argv[1]);
		confDebbuger(S.S_CONFIG);
        S.START();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return (0);
}
