#include "#MAIN.hpp"
#include SERVER_HPP
#include <csignal>

bool G_RUNNING = true;

void signalHandler(int sigCode) {
	std::cerr << "SIGNAL: " << sigCode << std::endl;
	G_RUNNING = false;
}

int main(int argc, char **argv) {

	signal(SIGINT, signalHandler);
	signal(SIGPIPE, SIG_IGN);

	if (argc != 2)
	{
		std::cerr << "Error: Invalid Argument" << std::endl;
		std::cerr << "Usage: ./webserv config.info" << std::endl;
		return (1);
	}

	try {
		Server S(argv[1]);
		S.START();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
