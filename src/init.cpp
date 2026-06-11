/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:34:18 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 05:37:51 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserver.hpp"

void initStuff(t_config *conf)
{
    conf->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    std::memset(&conf->address, 0, sizeof(conf->address));
    conf->address.sin_family = AF_INET;
    conf->address.sin_port = htons(8089);
    conf->address.sin_addr.s_addr = INADDR_ANY;
    conf->clientFd = 0;
}