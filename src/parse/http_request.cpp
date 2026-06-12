/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 15:16:53 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 15:40:46 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/webserver.hpp"

std::vector<std::string> tokenizeHttpRequest(const std::string& request)
{
    std::vector<std::string> tokens;
    std::string current;
    for (size_t i = 0; i < request.size(); ++i)
    {
        char c = request[i];
        if (c == '\r' && i + 1 < request.size() && request[i + 1] == '\n')
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back("\\r\\n");
            ++i;
        }
        else if (std::isspace(static_cast<unsigned char>(c)))
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        else if (c == ':')
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(":");
        }
        else
            current += c;
    }
    if (!current.empty())
        tokens.push_back(current);
    return (tokens);
}

void httpRequest::httpRequestDebbuger(httpRequest::request *req, int clientFd)
{
    log("RECEIVE", "Waiting for request...");
        char buffer[4096];
        int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
        buffer[bytes] = '\0';
        std::cout << "==== REQUEST ====\n" << buffer << "=================" << std::endl;
    req->tokens = tokenizeHttpRequest(static_cast<char*>(buffer));
    // std::cout << "===== TOKENIZED REQUEST FROM BROWSER =====" << std::endl;
    // for (size_t i = 0; i < req->tokens.size(); i++)
    //     std::cout << "[" << req->tokens[i] << "]" << std::endl;
    // std::cout << "===== FINISHED TOKENIZED REQUEST FROM BROSERR ======" << std::endl;
}