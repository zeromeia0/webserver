#include "_debug.hpp"

void debugConfigs(sConfigs *configs) {
std::cout << "========== PRINTING SERVER CONFIG VALUES ==========" << std::endl;
    for (size_t i = 0; i < configs->listenPorts.size(); i++)
        std::cout << "[Listen]: " << configs->listenPorts[i] << std::endl;
    std::cout << "[Server name]: " << configs->serverName << std::endl;
    std::cout << "[Host]: " << configs->host << std::endl;
    std::cout << "[Client Max Body Size]: " << configs->clientMaxBodySize << std::endl;

    std::cout << "[Error Pages]:" << std::endl;
    for (std::map<int, std::string>::iterator it = configs->errorPages.begin(); it != configs->errorPages.end(); ++it)
        std::cout << it->first << " -> " << it->second << std::endl;

    std::cout << "[Locations Count]: " << configs->router.size() << std::endl;

    for (size_t i = 0; i < configs->router.size(); i++)
    {
        std::cout << "\n[Location][" << i + 1 << "]: " << configs->router[i].path << std::endl;
        std::cout << "[Root]: " << configs->router[i].root << std::endl;
        std::cout << "[Index]: " << configs->router[i].index << std::endl;
        std::cout << "[Autoindex]: " << configs->router[i].autoindex << std::endl;
        std::cout << "[Upload Enabled]: " << configs->router[i].uploadEnabled << std::endl;
        std::cout << "[Upload Store]: " << configs->router[i].uploadStore << std::endl;
        std::cout << "[Redirect]: " << configs->router[i].redirect << std::endl;

        std::cout << "[Methods]: ";
        for (size_t j = 0; j < configs->router[i].methods.size(); j++)
            std::cout << configs->router[i].methods[j] << ((configs->router[i].methods.begin() + j + 1) != configs->router[i].methods.end() ? ", " : "");
        std::cout << std::endl;

        if (!configs->router[i].cgi.empty())
        {
            std::cout << "[CGI]:" << std::endl;
            for (std::map<std::string, std::string>::iterator it = configs->router[i].cgi.begin(); it != configs->router[i].cgi.end(); ++it)
                std::cout << it->first << " -> " << it->second << std::endl;
        }
    }
    std::cout << "========== FINISHED PRINTING SERVER CONFIG VALUES ==========" << std::endl;
}