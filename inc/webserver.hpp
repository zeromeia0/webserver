/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:12:41 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 14:39:12 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <algorithm>
# include <cstring>
# include <exception>
# include <fcntl.h>
# include <iomanip>
# include <iostream>
# include <list>
# include <map>
# include <netinet/in.h>
# include <stack>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>

class Server
{
public:
    struct serverInfo
    {
        int serverFd;
        int clientFd;
        sockaddr_in address;
    };

    struct routeConfig
    {
        routeConfig() : autoindex(false), uploadEnabled(false) {}

        std::string path;
        std::string root;
        std::string index;
        std::string uploadPath;
        std::string redirect;
        bool autoindex;
        bool uploadEnabled;
        std::vector<std::string> methods;
        std::map<std::string, std::string> cgi;
    };

    struct serverConfig
    {
        int listenPort;
        std::string host;
        std::string serverName;
        size_t clientMaxBodySize;
        std::map<int, std::string> errorPages;
        std::vector<routeConfig> router;
        std::vector<std::string> confFile;
    };

    Server();
    ~Server() {}
    void start(serverInfo *conf, char **argv);
};

void			initServer(Server::serverInfo *conf);
void			log(std::string category, std::string msg);
void assignValue(Server::serverConfig &server, const std::vector<std::string> &tokens, size_t i);
void getInfo(Server::serverConfig *conf, char **argv);
void parse(Server::serverConfig *conf, char **argv);
void initRouter(Server::routeConfig *conf);
void debbuger(Server::serverConfig *conf);

#endif