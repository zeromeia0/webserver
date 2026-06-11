/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:16:03 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 05:31:40 by vvazzs           ###   ########.fr       */
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

void Server::start(int serverFd, sockaddr_in address, char **argv)
{
    log("START", "Server starting...");
    log("CONFIG", argv[1]);
    
    log("SOCKET", "Creating socket...");
    std::cout << "[SOCKET] fd: " << serverFd << std::endl;
    
    
    log("BIND", "Binding to port 8089...");
    bind(serverFd, (sockaddr *)&address, sizeof(address));
    
    log ("LITEN", "Listening...");
    listen(serverFd, 10); // maybe here is where we accept a ton of requests
}