/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 20:45:19 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 20:53:04 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/webserver.hpp"

std::string validateFile(char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        throw (std::runtime_error("Can't open file"));
    char buffer[2048];
    std::string fileContent;
    int bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
        fileContent.append(buffer, bytes);
    close(fd);
    return (fileContent);
}

void validateContent(char **argv)
{
    (void)argv;
}

std::vector<std::string> tokenize(const std::string& file)
{
    std::vector<std::string> tokens;
    std::string current;

    for (size_t i = 0; i < file.size(); i++)
    {
        char c = file[i];
        if (std::isspace(c))
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        else if (c == '{' || c == '}' || c == ';')
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1, c));
        }
        else
            current += c;
    }
    if (!current.empty())
        tokens.push_back(current);
    return (tokens);
}

void getInfo(Server::serverConfig *conf, char **argv)
{
    // std::cout << "==== PRINTING CONF FILE ====\n";
    // std::cout << file << std::endl;
    // std::cout << "============================\n";
    (void)argv;
    std::cout << "==== TOKENIZED FILE ====\n";
    for (size_t i = 0; i < conf->confFile.size(); i++)
        std::cout << "[" << conf->confFile[i] << "]" << std::endl;
    std::cout << "========================\n";
}

void confAssignValue(Server::serverConfig *server, Server::routeConfig *&currentRoute, const std::vector<std::string> &tokens, size_t i)
{
    if (tokens[i] == "listen")
        server->listenPort = atoi(tokens[i + 1].c_str());
    else if (tokens[i] == "host")
        server->host = tokens[i + 1];
    else if (tokens[i] == "server_name")
        server->serverName = tokens[i + 1];
    else if (tokens[i] == "client_max_body_size")
        server->clientMaxBodySize = atoi(tokens[i + 1].c_str());
    else if (tokens[i] == "error_page")
        server->errorPages[atoi(tokens[i + 1].c_str())] = tokens[i + 2];
    else if (tokens[i] == "location")
    {
        Server::routeConfig route;
        route.path = tokens[i + 1];
        server->router.push_back(route);
        currentRoute = &server->router.back();
    }
    else if (currentRoute)
    {
        if (tokens[i] == "root")
            currentRoute->root = tokens[i + 1];
        else if (tokens[i] == "index")
            currentRoute->index = tokens[i + 1];
        else if (tokens[i] == "autoindex")
            currentRoute->autoindex = (tokens[i + 1] == "on");
        else if (tokens[i] == "upload_enabled")
            currentRoute->uploadEnabled = (tokens[i + 1] == "on");
        else if (tokens[i] == "upload_store")
            currentRoute->uploadPath = tokens[i + 1];
        else if (tokens[i] == "redirect")
            currentRoute->redirect = tokens[i + 1];
        else if (tokens[i] == "allowed_methods")
        {
            size_t j = i + 1;
            while (j < tokens.size() && tokens[j] != ";")
            {
                currentRoute->methods.push_back(tokens[j]);
                j++;
            }
        }
        else if (tokens[i] == "cgi")
            currentRoute->cgi[tokens[i + 1]] = tokens[i + 2];
    }
}