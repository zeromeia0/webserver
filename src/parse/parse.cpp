/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 06:54:37 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 21:05:10 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/webserver.hpp"

void parse(Server::serverConfig *conf, char **argv)
{
    std::string file = validateFile(argv);
    conf->confFile = tokenize(file);
    validateSyntax(conf->confFile);
    getInfo(conf, argv);
    Server::routeConfig *currentRoute = NULL;
    for (size_t i = 0; i < conf->confFile.size(); i++)
        confAssignValue(conf, currentRoute, conf->confFile, i);
    confDebbuger(conf);
}