/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 06:54:37 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 12:20:21 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/webserver.hpp"

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
        {
            current += c;
        }
    }

    if (!current.empty())
        tokens.push_back(current);

    return tokens;
}

void getInfo(t_serverConfig *conf, char **argv)
{
    std::string file = validateFile(argv);
    // std::cout << "==== PRINTING CONF FILE ====\n";
    // std::cout << file << std::endl;
    // std::cout << "============================\n";
    conf->confFile = tokenize(file);
    // std::cout << "==== TOKENIZED FILE ====\n";
    // for (size_t i = 0; i < conf->confFile.size(); i++)
    //     std::cout << "[" << conf->confFile[i] << "]" << std::endl;
    // std::cout << "========================\n";
}

void assignValue(t_serverConfig &server, t_routeConfig *&currentRoute, const std::vector<std::string> &tokens, size_t i)
{
    if (tokens[i] == "listen")
        server.listenPort = atoi(tokens[i + 1].c_str());
    else if (tokens[i] == "host")
        server.host = tokens[i + 1];
    else if (tokens[i] == "server_name")
        server.serverName = tokens[i + 1];
    else if (tokens[i] == "client_max_body_size")
        server.clientMaxBodySize = atoi(tokens[i + 1].c_str());
    else if (tokens[i] == "error_page")
        server.errorPages[atoi(tokens[i + 1].c_str())] = tokens[i + 2];
    else if (tokens[i] == "location")
    {
        t_routeConfig route;
        route.path = tokens[i + 1];
        server.router.push_back(route);
        currentRoute = &server.router.back();
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

void parse(t_serverConfig *conf, char **argv)
{
    getInfo(conf, argv);

    t_routeConfig *currentRoute = NULL;

    for (size_t i = 0; i < conf->confFile.size(); i++)
        assignValue(*conf, currentRoute, conf->confFile, i);

    std::cout << "========== PRINTING SERVER CONFIG VALUES ==========" << std::endl;
    std::cout << "[Listen]: " << conf->listenPort << std::endl;
    std::cout << "[Server name]: " << conf->serverName << std::endl;
    std::cout << "[Host]: " << conf->host << std::endl;
    std::cout << "[Client Max Body Size]: " << conf->clientMaxBodySize << std::endl;

    std::cout << "[Error Pages]:" << std::endl;
    for (std::map<int, std::string>::iterator it = conf->errorPages.begin(); it != conf->errorPages.end(); ++it)
        std::cout << it->first << " -> " << it->second << std::endl;

    std::cout << "[Locations Count]: " << conf->router.size() << std::endl;

    for (size_t i = 0; i < conf->router.size(); i++)
    {
        std::cout << "\n[Location][" << i + 1 << "]: " << conf->router[i].path << std::endl;
        std::cout << "[Root]: " << conf->router[i].root << std::endl;
        std::cout << "[Index]: " << conf->router[i].index << std::endl;
        std::cout << "[Autoindex]: " << conf->router[i].autoindex << std::endl;
        std::cout << "[Upload Enabled]: " << conf->router[i].uploadEnabled << std::endl;
        std::cout << "[Upload Store]: " << conf->router[i].uploadPath << std::endl;
        std::cout << "[Redirect]: " << conf->router[i].redirect << std::endl;

        std::cout << "[Methods]: ";
        for (size_t j = 0; j < conf->router[i].methods.size(); j++)
            std::cout << conf->router[i].methods[j] << " ";
        std::cout << std::endl;

        if (!conf->router[i].cgi.empty())
        {
            std::cout << "[CGI]:" << std::endl;
            for (std::map<std::string, std::string>::iterator it = conf->router[i].cgi.begin(); it != conf->router[i].cgi.end(); ++it)
                std::cout << it->first << " -> " << it->second << std::endl;
        }
    }

    std::cout << "========== FINISHED PRINTING SERVER CONFIG VALUES ==========" << std::endl;
}

