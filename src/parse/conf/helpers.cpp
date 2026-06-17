/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 08:18:24 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/17 08:19:19 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/webserver.hpp"

bool isDirective(const std::string &token)
{
    return (token == "server" || token == "listen" || token == "host" || token == "server_name" ||
            token == "client_max_body_size" || token == "error_page" || token == "location" ||
            token == "root" || token == "index" || token == "autoindex" || token == "allowed_methods" ||
            token == "upload_enabled" || token == "upload_store" || token == "redirect" || token == "cgi");
}

bool isNumber(const std::string &token)
{
    if (token.empty())
        return (false);
    for (size_t i = 0; i < token.size(); i++)
    {
        if (!std::isdigit(static_cast<unsigned char>(token[i])))
            return (false);
    }
    return (true);
}

bool isMethod(const std::string &token)
{
    return (token == "GET" || token == "POST" || token == "DELETE");
}

size_t findSemicolon(const std::vector<std::string> &tokens, size_t start)
{
    size_t i = start;
    while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}")
        i++;
    if (i >= tokens.size() || tokens[i] != ";")
        throw std::runtime_error("Missing semicolon after directive: " + tokens[start]);
    return (i);
}

size_t countArgsUntilSemicolon(const std::vector<std::string> &tokens, size_t start)
{
    size_t end = findSemicolon(tokens, start);
    if (end <= start)
        return (0);
    return (end - start - 1);
}

void expectArgs(const std::vector<std::string> &tokens, size_t i, size_t expected)
{
    size_t count = countArgsUntilSemicolon(tokens, i);
    if (count != expected)
        throw std::runtime_error("Invalid number of arguments in directive: " + tokens[i]);
}