/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:34:18 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 20:50:38 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserver.hpp"

void initServer(Server::serverInfo *conf)
{
    conf->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    std::memset(&conf->address, 0, sizeof(conf->address));
    conf->address.sin_family = AF_INET;
    conf->address.sin_port = htons(8089);
    conf->address.sin_addr.s_addr = INADDR_ANY;
    conf->clientFd = 0;
    // conf->listenPort = 0;
    // conf->rootDir.clear();
}

void initRouter(Server::routeConfig *conf)
{
    conf->autoindex = false;
    conf->cgi.clear();
    conf->index.clear();
    conf->methods.clear();
    conf->path.clear();
    conf->redirect.clear();
    conf->root.clear();
    conf->uploadEnabled = false;
    conf->uploadPath.clear();
}

void initStuff(Server::serverInfo *conf, Server::routeConfig *route)
{
    initServer(conf);
    initRouter(route);
}