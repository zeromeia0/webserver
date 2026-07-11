/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 12:24:17 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 15:16:20 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main.hpp"

void confDebbuger(Server::serverConfig *conf)
{
std::cout << "========== PRINTING SERVER CONFIG VALUES ==========" << std::endl;
    for (size_t i = 0; i < conf->listenPorts.size(); i++)
        std::cout << "[Listen]: " << conf->listenPorts[i] << std::endl;
    std::cout << "[Server name]: " << conf->serverName << std::endl;
    std::cout << "[Host]: " << conf->host << std::endl;
    std::cout << "[Client Max Body Size]: " << conf->clientMaxBodySize << std::endl;

    std::cout << "[Error Pages]:" << std::endl;
    for (std::map<int, std::string>::iterator it = conf->errorPages.begin(); it != conf->errorPages.end(); ++it)
        std::cout << it->first << " -> " << it->second << std::endl;

    std::cout << "[Locations Count]: " << conf->router.size() << std::endl;

    for (size_t i = 0; i < conf->router.size(); i++)
    {
        std::cout << "\n[Location][" << i + 1 << "]: " << conf->router[i].path << std::endl;
        std::cout << "[Root]: " << conf->router[i].root << std::endl;
        std::cout << "[Index]: " << conf->router[i].index << std::endl;
        std::cout << "[Autoindex]: " << conf->router[i].autoindex << std::endl;
        std::cout << "[Upload Enabled]: " << conf->router[i].uploadEnabled << std::endl;
        std::cout << "[Upload Store]: " << conf->router[i].uploadPath << std::endl;
        std::cout << "[Redirect]: " << conf->router[i].redirect << std::endl;

        std::cout << "[Methods]: ";
        for (size_t j = 0; j < conf->router[i].methods.size(); j++)
            std::cout << conf->router[i].methods[j] << ((conf->router[i].methods.begin() + j + 1) != conf->router[i].methods.end() ? ", " : "");
        std::cout << std::endl;

        if (!conf->router[i].cgi.empty())
        {
            std::cout << "[CGI]:" << std::endl;
            for (std::map<std::string, std::string>::iterator it = conf->router[i].cgi.begin(); it != conf->router[i].cgi.end(); ++it)
                std::cout << it->first << " -> " << it->second << std::endl;
        }
    }
    std::cout << "========== FINISHED PRINTING SERVER CONFIG VALUES ==========" << std::endl;
}