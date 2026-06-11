/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 06:54:37 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/11 07:01:56 by vvazzs           ###   ########.fr       */
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
        }
        else
            current += c;
    }
    if (!current.empty())
        tokens.push_back(current);
    return (tokens);
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

void assignValue(t_serverConfig &server, const std::vector<std::string> &tokens, size_t i)
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!Assign value function called\n";
    if (tokens[i] == "listen")
        server.listenPort = atoi(tokens[i + 1].c_str());

    else if (tokens[i] == "host")
        server.host = tokens[i + 1];

    else if (tokens[i] == "server_name")
        server.serverName = tokens[i + 1];

    else if (tokens[i] == "root")
        server.router[0].root = tokens[i + 1];

    else if (tokens[i] == "index")
        server.router[0].index = tokens[i + 1];

    else if (tokens[i] == "client_max_body_size")
        server.clientMaxBodySize = atoi(tokens[i + 1].c_str());

    else if (tokens[i] == "autoindex")
        server.router[0].autoindex = (tokens[i + 1] == "on");
}

void parse(t_serverConfig *conf, char **argv)
{
    getInfo(conf, argv);
    assignValue(*conf, conf->confFile, 0);
}