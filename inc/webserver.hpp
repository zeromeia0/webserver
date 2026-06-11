/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:12:41 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 05:41:50 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

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

typedef struct s_config
{
    int serverFd;
    int clientFd;
    sockaddr_in address;    
}   t_config;

class Server
{
    public:
        Server();
        ~Server() {};
        void start(t_config *conf, char **argv);
        
};

void initStuff(t_config *conf);

#endif