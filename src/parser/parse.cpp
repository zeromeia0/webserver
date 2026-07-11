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

#include "../main.hpp"

void parse(Server::serverConfig *conf, char *fileName)
{
    std::string file = validateFile(fileName);
    conf->confFile = tokenize(file);
    for (size_t i = 0; i < conf->confFile.size(); i++)
        LOG("DEBUG", conf->confFile[i]);
    validateSyntax(conf->confFile);
    // getInfo(conf);
    routeConfig *currentRoute = NULL;
    for (size_t i = 0; i < conf->confFile.size(); i++)
        confAssignValue(conf, currentRoute, conf->confFile, i);
    confDebbuger(conf);
}