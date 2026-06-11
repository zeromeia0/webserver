/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:10:17 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 05:31:14 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserver.hpp"

typedef struct s_config
{
    int serverFd;
    sockaddr_in address;    
}   t_config;

void initStuff(t_config *conf)
{
    conf->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    std::memset(&conf->address, 0, sizeof(conf->address));
    conf->address.sin_family = AF_INET;
    conf->address.sin_port = htons(8089);
    conf->address.sin_addr.s_addr = INADDR_ANY;
}

void log2(std::string category, std::string msg)
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
    t_config *conf = new t_config;
    Server server;
    initStuff(conf);
    server.start(conf->serverFd, conf->address, argv);
    
    log2 ("ACCEPT", "Waiting for browser...");
    int clientFd = accept(conf->serverFd, NULL, NULL);
    std::cout << "[ACCEPT] client fd: " << clientFd << std::endl;
    
    
    char buffer[4096];
    log2("RECEIVE", "Waiting for request...");
    int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';
    std::cout << "==== REQUEST ====\n" << buffer << "=================" << std::endl;
    
    const char *response = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>"
        "<h1>Hello Webserv</h1>"
        "</body></html>";

    log2("SEND", "Sending response...");
    send(clientFd, response, std::strlen(response), 0);
    close(clientFd);
    close(conf->serverFd);
    delete (conf);
    log2("END", "Server finished...");
        return (0);
    
}
