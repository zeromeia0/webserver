/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:10:17 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/17 08:40:22 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserver.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: Invalid Argument" << std::endl;
        std::cout << "Usage: ./webserv config.info" << std::endl;
        return (1);
    }
    try{
        
        Server server;
        Server::serverInfo *info = new Server::serverInfo;
        Server::serverConfig *conf = new Server::serverConfig;        
        Server::routeConfig *router = new Server::routeConfig;       
        httpRequest req;
        httpRequest::request *htreq = new httpRequest::request;
        
        initStuff(info, router, htreq);
        parse(conf, argv);
        server.start(info, argv);
        req.httpRequestDebbuger(htreq, info->clientFd);

        
        const char *response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "<html><body>"
            "<h1>Hello Webserv</h1>"
            "</body></html>";
        
        log("SEND", "Sending response...");
        send(info->clientFd, response, std::strlen(response), 0);
        close(info->clientFd);
        close(info->serverFd);
        delete (info);
        log("END", "Server finished...");
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
        return (0);
    
}
