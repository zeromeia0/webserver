/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:10:17 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/09 16:28:25 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <exception>
#include <stack>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void log(std::string category, std::string msg)
{
    std::cout
        << "[" << category << "] "
        << msg
        << std::endl;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: Invalid Argument" << std::endl;
        std::cout << "Usage: ./webserv config.conf" << std::endl;
        return (1);
    }
    
    log("START", "Server starting...");
    log("CONFIG", argv[1]);
    
    log("SOCKET", "Creating socket...");
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "[SOCKET] fd: " << serverFd << std::endl;
    
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(8089);
    address.sin_addr.s_addr = INADDR_ANY;
    
    log("BIND", "Binding to port 8089...");
    bind(serverFd, (sockaddr *)&address, sizeof(address));
    
    log ("LITEN", "Listening...");
    listen(serverFd, 10); // maybe here is where we accept a ton of requests
    
    log ("ACCEPT", "Waiting for browser...");
    int clientFd = accept(serverFd, NULL, NULL);
    std::cout << "[ACCEPT] client fd: " << clientFd << std::endl;
    
    
    char buffer[4096];
    log("RECEIVE", "Waiting for request...");
    int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';
    std::cout << "==== REQUEST ====\n" << buffer << "=================" << std::endl;
    
    const char *response = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>"
        "<h1>Hello Webserv</h1>"
        "</body></html>";

        log("SEND", "Sending response...");
        send(clientFd, response, std::strlen(response), 0);
        close(clientFd);
        close(serverFd);
        log("END", "Server finished...");
        return (0);
    
}
