/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_validation.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 21:00:00 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/17 08:21:18 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../main.hpp"

void checkBraces(const std::vector<std::string> &tokens)
{
    int depth = 0;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "{")
            depth++;
        else if (tokens[i] == "}")
            depth--;
        if (depth < 0)
            throw (std::runtime_error("Extra closing brace"));
    }
    if (depth != 0)
        throw (std::runtime_error("Missing closing brace"));
}

void checkServerBlock(const std::vector<std::string> &tokens)
{
    bool foundServer = false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "server")
        {
            foundServer = true;
            if (i + 1 >= tokens.size() || tokens[i + 1] != "{")
                throw (std::runtime_error("server block must be followed by '{'"));
        }
    }
    if (!foundServer)
        throw (std::runtime_error("Missing server block"));
}

void checkUnknownDirectives(const std::vector<std::string>& tokens)
{
    for (size_t i = 0; i < tokens.size(); )
    {
        if (tokens[i] == "{" || tokens[i] == "}" || tokens[i] == ";")
        {
            ++i;
            continue;
        }
        if (tokens[i] == "server")
        {
            i += 2;
            continue;
        }
        if (tokens[i] == "location")
        {
            i += 3;
            continue;
        }
        if (!isDirective(tokens[i]))
            throw (std::runtime_error("Unknown directive: " + tokens[i]));
        ++i;
        while (i < tokens.size() && tokens[i] != ";")
            ++i;
        if (i < tokens.size())
            ++i;
    }
}

void checkDuplicates(const std::vector<std::string>& tokens)
{
    std::set<std::string> serverDirectives;
    std::set<std::string> locationDirectives;
    std::set<std::string> locationPaths;
    bool inServer = false;
    bool inLocation = false;
    int depth = 0;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i] == "server")
        {
            serverDirectives.clear();
            locationPaths.clear();
            inServer = true;
        }
        else if (tokens[i] == "{")
            ++depth;
        else if (tokens[i] == "}")
        {
            if (inLocation && depth == 2)
            {
                inLocation = false;
                locationDirectives.clear();
            }
            --depth;
            if (depth == 0)
                inServer = false;
        }
        else if (tokens[i] == "location")
        {
            std::string path = tokens[i + 1];
            if (locationPaths.count(path))
                throw (std::runtime_error("Duplicate location: " + path));
            locationPaths.insert(path);
            locationDirectives.clear();
            inLocation = true;
        }
        else if (inServer && !inLocation)
        {
            if (tokens[i] == "host" ||
                tokens[i] == "server_name" ||
                tokens[i] == "client_max_body_size")
            {
                if (serverDirectives.count(tokens[i]))
                    throw (std::runtime_error("Duplicate directive: " + tokens[i]));
                serverDirectives.insert(tokens[i]);
            }
        }
        else if (inLocation)
        {
            if (tokens[i] == "root" ||
                tokens[i] == "index" ||
                tokens[i] == "autoindex" ||
                tokens[i] == "allowed_methods" ||
                tokens[i] == "upload_enabled" ||
                tokens[i] == "upload_store" ||
                tokens[i] == "redirect")
            {
                if (locationDirectives.count(tokens[i]))
                    throw (std::runtime_error("Duplicate directive in location: " + tokens[i]));
                locationDirectives.insert(tokens[i]);
            }
        }
    }
}

void checkDirectiveContext(const std::vector<std::string> &tokens)
{
    int depth = 0;
    bool inLocation = false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "{")
            depth++;
        else if (tokens[i] == "}")
        {
            if (inLocation && depth == 2)
                inLocation = false;
            depth--;
        }
        else if (tokens[i] == "location")
        {
            if (depth != 1)
                throw std::runtime_error("location directive must be inside server block");
            inLocation = true;
        }
        else if (tokens[i] == "listen" || tokens[i] == "host" || tokens[i] == "server_name" || tokens[i] == "client_max_body_size" || tokens[i] == "error_page")
        {
            if (depth != 1)
                throw std::runtime_error("Server directive in wrong context: " + tokens[i]);
        }
        else if (tokens[i] == "root" || tokens[i] == "index" || tokens[i] == "autoindex" || tokens[i] == "allowed_methods" || tokens[i] == "upload_enabled" || tokens[i] == "upload_store" || tokens[i] == "redirect" || tokens[i] == "cgi")
        {
            if (depth != 2 || !inLocation)
                throw std::runtime_error("Route directive in wrong context: " + tokens[i]);
        }
    }
}

void checkDirectiveArguments(const std::vector<std::string> &tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "server")
        {
            if (i + 1 >= tokens.size() || tokens[i + 1] != "{")
                throw std::runtime_error("Invalid server block");
        }
        else if (tokens[i] == "location")
        {
            if (i + 2 >= tokens.size() || tokens[i + 2] != "{")
                throw std::runtime_error("Invalid location block");
            if (tokens[i + 1].empty() || tokens[i + 1][0] != '/')
                throw std::runtime_error("Invalid location path");
        }
        else if (tokens[i] == "listen" || tokens[i] == "host" || tokens[i] == "server_name" || tokens[i] == "client_max_body_size" || tokens[i] == "root" || tokens[i] == "index" || tokens[i] == "autoindex" || tokens[i] == "upload_enabled" || tokens[i] == "upload_store" || tokens[i] == "redirect")
            expectArgs(tokens, i, 1);
        else if (tokens[i] == "error_page")
            expectArgs(tokens, i, 2);
        else if (tokens[i] == "cgi")
            expectArgs(tokens, i, 2);
        else if (tokens[i] == "allowed_methods")
        {
            size_t count = countArgsUntilSemicolon(tokens, i);
            if (count < 1)
                throw std::runtime_error("allowed_methods needs at least one method");
        }
    }
}

void checkSemicolons(const std::vector<std::string> &tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "server" || tokens[i] == "location" || tokens[i] == "{" || tokens[i] == "}" || tokens[i] == ";")
            continue;
        if (isDirective(tokens[i]))
            findSemicolon(tokens, i);
    }
}

void checkValues(const std::vector<std::string> &tokens)
{
    bool hasListen = false;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "listen")
        {
            hasListen = true;
            if (!isNumber(tokens[i + 1]))
                throw std::runtime_error("Invalid listen port: " + tokens[i + 1]);
            int port = atoi(tokens[i + 1].c_str());
            if (port < 1 || port > 65535)
                throw std::runtime_error("Listen port out of range");
        }
        else if (tokens[i] == "client_max_body_size")
        {
            if (!isNumber(tokens[i + 1]))
                throw std::runtime_error("Invalid client_max_body_size");
        }
        else if (tokens[i] == "error_page")
        {
            if (!isNumber(tokens[i + 1]))
                throw std::runtime_error("Invalid error code");
            int code = atoi(tokens[i + 1].c_str());
            if (code < 100 || code > 599)
                throw std::runtime_error("Error code out of range");
        }
        else if (tokens[i] == "autoindex")
        {
            if (tokens[i + 1] != "on" && tokens[i + 1] != "off")
                throw std::runtime_error("autoindex must be on or off");
        }
        else if (tokens[i] == "upload_enabled")
        {
            if (tokens[i + 1] != "on" && tokens[i + 1] != "off")
                throw std::runtime_error("upload_enabled must be on or off");
        }
        else if (tokens[i] == "allowed_methods")
        {
            size_t j = i + 1;
            while (j < tokens.size() && tokens[j] != ";")
            {
                if (!isMethod(tokens[j]))
                    throw std::runtime_error("Invalid HTTP method: " + tokens[j]);
                j++;
            }
        }
        else if (tokens[i] == "location")
        {
            if (tokens[i + 1].empty())
                throw std::runtime_error("Empty location path");
            if (tokens[i + 1][0] != '/')
                throw std::runtime_error("Location path must start with '/'");
        }
        else if (tokens[i] == "cgi")
        {
            if (tokens[i + 1].empty() || tokens[i + 2].empty())
                throw std::runtime_error("Invalid CGI declaration");
            if (tokens[i + 1][0] != '.')
                throw std::runtime_error("CGI extension must start with '.'");
        }
    }
    if (!hasListen)
        throw std::runtime_error("Missing listen directive");
}

void validateSyntax(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        throw std::runtime_error("Empty configuration file");
    checkBraces(tokens);
    checkUnknownDirectives(tokens);
    checkServerBlock(tokens);
    checkDuplicates(tokens);
    checkSemicolons(tokens);
    checkDirectiveContext(tokens);
    checkDirectiveArguments(tokens);
    checkValues(tokens);
}