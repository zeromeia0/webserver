#include "_debug.hpp"

void debugConfigs(sConfigs *configs) {
	std::cerr << "========== PRINTING SERVER CONFIG VALUES ==========" << std::endl;
	// std::cerr << "[Listen]: " << configs->listenPorts << std::endl;
	std::cerr << "[Server name]: " << configs->serverName << std::endl;
	std::cerr << "[Host]: " << configs->host << std::endl;
	std::cerr << "[Client Max Body Size]: " << configs->clientMaxBodySize << std::endl;

	std::cerr << "[Error Pages]:" << std::endl;
	for (std::map<int, std::string>::iterator it = configs->errorPages.begin(); it != configs->errorPages.end(); ++it)
		std::cerr << it->first << " -> " << it->second << std::endl;

	std::cerr << "[Locations Count]: " << configs->router.size() << std::endl;

	for (size_t i = 0; i < configs->router.size(); i++)
	{
		std::cerr << "\n[Location][" << i + 1 << "]: " << configs->router[i].path << std::endl;
		std::cerr << "[Root]: " << configs->router[i].root << std::endl;
		std::cerr << "[Index]: " << configs->router[i].index << std::endl;
		std::cerr << "[Autoindex]: " << configs->router[i].autoindex << std::endl;
		std::cerr << "[Upload Enabled]: " << configs->router[i].uploadEnabled << std::endl;
		std::cerr << "[Upload Store]: " << configs->router[i].uploadStore << std::endl;
		std::cerr << "[Redirect]: " << configs->router[i].redirect << std::endl;

		std::cerr << "[Methods]: ";
		for (size_t j = 0; j < configs->router[i].methods.size(); j++)
			std::cerr << configs->router[i].methods[j] << ((configs->router[i].methods.begin() + j + 1) != configs->router[i].methods.end() ? ", " : "");
		std::cerr << std::endl;

		if (!configs->router[i].cgi.empty())
		{
			std::cerr << "[CGI]:" << std::endl;
			for (std::map<std::string, std::string>::iterator it = configs->router[i].cgi.begin(); it != configs->router[i].cgi.end(); ++it)
				std::cerr << it->first << " -> " << it->second << std::endl;
		}
	}
	std::cerr << "========== FINISHED PRINTING SERVER CONFIG VALUES ==========" << std::endl;
}