/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:12:41 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 07:02:04 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <algorithm>
# include <cstring>
# include <exception>
# include <fcntl.h>
# include <iomanip>
# include <iostream>
# include <list>
# include <map>
# include <netinet/in.h>
# include <stack>
# include <sys/socket.h>
# include <unistd.h>
# include <vector>

typedef struct s_serverInfo
{
	int			serverFd;
	int			clientFd;
	sockaddr_in	address;
}				t_serverInfo;

typedef struct s_routeConfig
{
	std::string                         path;
	std::string                         root;
	std::string                         index;
	std::string                         uploadPath;
	std::string                         redirect;
	bool		                        autoindex;
	bool		                        uploadEnabled;
	std::vector<std::string>            methods;
	std::map<std::string, std::string>  cgi;
}				t_routeConfig;

typedef struct s_serverConfig
{
	int			listenPort;
	std::string host;
	std::string serverName;
	size_t		clientMaxBodySize;
	std::map<int, std::string> errorPages;
	std::vector<t_routeConfig> router;

	std::vector<std::string> confFile;
}				t_serverConfig;

class Server
{
  public:
	Server();
	~Server(){};
	void start(t_serverInfo *conf, char **argv);
};

void			initStuff(t_serverInfo *conf);
void			log(std::string category, std::string msg);
void assignValue(t_serverConfig &server, const std::vector<std::string> &tokens, size_t i);
void getInfo(t_serverConfig *conf, char **argv);
void parse(t_serverConfig *conf, char **argv);

#endif