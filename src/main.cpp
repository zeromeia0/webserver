/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:10:17 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 05:43:34 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserver.hpp"

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
    server.start(conf, argv);
    
    
    // log2 ("ACCEPT", "Waiting for browser...");
    // int clientFd = accept(conf->serverFd, NULL, NULL);
    // std::cout << "[ACCEPT] client fd: " << clientFd << std::endl;
    
    
    char buffer[4096];
    log2("RECEIVE", "Waiting for request...");
    int bytes = recv(conf->clientFd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';
    std::cout << "==== REQUEST ====\n" << buffer << "=================" << std::endl;
    
    const char *response = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body>"
        "<h1>Hello Webserv</h1>"
        "</body></html>";

    log2("SEND", "Sending response...");
    send(conf->clientFd, response, std::strlen(response), 0);
    close(conf->clientFd);
    close(conf->serverFd);
    delete (conf);
    log2("END", "Server finished...");
        return (0);
    
}
