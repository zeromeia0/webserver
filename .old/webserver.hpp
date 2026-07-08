 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 05:12:41 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 21:04:02 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #ifndef WEBSERVER_HPP
// # define WEBSERVER_HPP

// # include <algorithm>
// # include <cstring>
// # include <exception>
// # include <fcntl.h>
// # include <iomanip>
// # include <iostream>
// # include <list>
// # include <map>
// # include <netinet/in.h>
// # include <stack>
// # include <sys/socket.h>
// # include <unistd.h>
// # include <vector>
// # include <set>
// #include "httpRequest.hpp"

// class Server
// {
// public:
//     struct serverInfo
//     {
//         int serverFd;
//         int clientFd;
//         sockaddr_in address;
//     };

//     struct routeConfig
//     {
//         routeConfig() : autoindex(false), uploadEnabled(false) {}
//         std::string path;
//         std::string root;
//         std::string index;
//         std::string uploadPath;
//         std::string redirect;
//         bool autoindex;
//         bool uploadEnabled;
//         std::vector<std::string> methods;
//         std::map<std::string, std::string> cgi;
//     };

//     struct serverConfig
//     {
//         int listenPort;
//         std::string host;
//         std::string serverName;
//         size_t clientMaxBodySize;
//         std::map<int, std::string> errorPages;
//         std::vector<routeConfig> router;
//         std::vector<std::string> confFile;
//     };

//     Server();
//     ~Server() {}
//     void start(serverInfo *conf, char **argv);
// };

// void			initServer(Server::serverInfo *conf);
// void 			parse(Server::serverConfig *conf, char **argv);
// void 			initRouter(Server::routeConfig *conf);
// void initStuff(Server::serverInfo *conf, Server::routeConfig *route);

// // PARSING .CONF SHIT
// std::string validateFile(char **argv);
// void 			getInfo(Server::serverConfig *conf, char **argv);
// void 			confDebbuger(Server::serverConfig *conf);
// void            confAssignValue(Server::serverConfig *server, Server::routeConfig *&currentRoute, const std::vector<std::string> &tokens, size_t i);
// std::vector<std::string> tokenizeHttpRequest(const std::string& file); //TRY TO POLYMORPHISM THIS SHIT

// // TRYING OUT NOW
// void validateSemantics(const std::vector<std::string> &tokens);
// void checkDirectiveArguments(const std::vector<std::string> &tokens);
// void checkListen(const Server::serverConfig *conf);
// void checkMethods(const Server::serverConfig *conf);
// void checkRoutes(const Server::serverConfig *conf);
// void checkCgi(const Server::serverConfig *conf);

// void validateSyntax(const std::vector<std::string> &tokens);
// void checkBraces(const std::vector<std::string> &tokens);
// void checkServerBlock(const std::vector<std::string> &tokens);
// void checkUnknownDirectives(const std::vector<std::string> &tokens);
// void checkDirectiveContext(const std::vector<std::string> &tokens);
// void checkDirectiveArguments(const std::vector<std::string> &tokens);
// void checkSemicolons(const std::vector<std::string> &tokens);
// void checkValues(const std::vector<std::string> &tokens);

// bool isDirective(const std::string &token);
// bool isNumber(const std::string &token);
// bool isMethod(const std::string &token);

// size_t findSemicolon(const std::vector<std::string> &tokens, size_t start);
// size_t countArgsUntilSemicolon(const std::vector<std::string> &tokens, size_t start);
// void expectArgs(const std::vector<std::string> &tokens, size_t i, size_t expected);
// void checkBraces(const std::vector<std::string> &tokens);
// void checkServerBlock(const std::vector<std::string> &tokens);
// void checkDirectiveContext(const std::vector<std::string> &tokens);
// void checkDirectiveArguments(const std::vector<std::string> &tokens);
// void checkSemicolons(const std::vector<std::string> &tokens);
// void checkValues(const std::vector<std::string> &tokens);
// void validateSyntax(const std::vector<std::string> &tokens);
// void checkDuplicates(const std::vector<std::string>& tokens);



// #endif