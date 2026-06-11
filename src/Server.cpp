/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:16:03 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 06:49:29 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserver.hpp"

void log(std::string category, std::string msg)
{
    std::cout
        << "[" << category << "] "
        << msg
        << std::endl;
}

Server::Server() {};

void Server::start(t_serverInfo *conf, char **argv)
{
    log("START", "Server starting...");
    log("CONFIG", argv[1]);
    
    log("SOCKET", "Creating socket...");
    std::cout << "[SOCKET] fd: " << conf->serverFd << std::endl;
    
    
    log("BIND", "Binding to port 8089...");
    bind(conf->serverFd, (sockaddr *)&conf->address, sizeof(conf->address));
    
    log ("LITEN", "Listening...");
    listen(conf->serverFd, 10); // maybe here is where we accept a ton of requests

    log ("ACCEPT", "Waiting for browser...");
    conf->clientFd = accept(conf->serverFd, NULL, NULL);
    std::cout << "[ACCEPT] client fd: " << conf->clientFd << std::endl;
}