/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 15:16:53 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/18 08:40:07 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/webserver.hpp"

void parseRequest(const std::string& raw, httpRequest::request *req)
{
    std::istringstream stream(raw);
    std::string line;

    if (!std::getline(stream, line))
        throw (std::runtime_error("Empty request"));
    if (!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);
    std::istringstream requestLine(line);
    if (!(requestLine >> req->method >> req->uri >> req->version))
        throw (std::runtime_error("Invalid request line"));
    while (std::getline(stream, line))
    {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
        if (line.empty())
            break;
        size_t pos = line.find(':');
        if (pos == std::string::npos)
            throw (std::runtime_error("Invalid header"));
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        while (!value.empty() && value[0] == ' ')
            value.erase(0, 1);
        req->headers[key] = value;
    }
    while (std::getline(stream, line))
        req->body += line;
}

void httpRequest::httpRequestDebbuger(httpRequest::request *req, int clientFd)
{
    log("RECEIVE", "Waiting for request...");
    char buffer[4096];
    int bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
        return;
    buffer[bytes] = '\0';
    std::string raw(buffer);
    std::cout << "\n===== RAW REQUEST =====\n";
    std::cout << raw;
    std::cout << "\n=======================\n";
    parseRequest(raw, req);
    std::cout << "\n===== PARSED REQUEST =====\n";
    std::cout << "Method  : " << req->method << std::endl;
    std::cout << "URI     : " << req->uri << std::endl;
    std::cout << "Version : " << req->version << std::endl;
    std::cout << "\nHeaders:\n";
    for (std::map<std::string, std::string>::iterator it = req->headers.begin();
         it != req->headers.end();
         ++it)
    {
        std::cout << "  " << it->first
                  << " => "
                  << it->second
                  << std::endl;
    }
    if (!req->body.empty())
    {
        std::cout << "\nBody:\n";
        std::cout << req->body << std::endl;
    }
    std::cout << "==========================\n";
}

void checkHost(httpRequest::request *req, std::string *hostname, int port)
{
    std::map<std::string, std::string>::iterator it = req->headers.find("Host");
    if (it == req->headers.end())
        throw (std::runtime_error("400 Bad Request"));
    std::string host = it->second, hostname;
    size_t pos = host.find(':');
    std::map<std::string, std::string>::iterator it = req->headers.find("Host");
    if (it == req->headers.end())
        throw (std::runtime_error("400 Bad Request"));
    std::string host = it->second, hostname;
    int port;
    size_t pos = host.find(':');
    if (pos == std::string::npos)
    {
        *hostname = host;
        port = 80;
    }
    else
    {
        *hostname = host.substr(0, pos);
        port = atoi(host.substr(pos + 1).c_str());
    }
}

void httpRequest::parse(httpRequest::request *req, Server::serverConfig *conf, int clientFd)
{
    httpRequestDebbuger(req, clientFd);
    std::string hostname;
    int port;

    if (req->method != "GET" && req->method != "POST" && req->method != "DELETE")
        throw (std::runtime_error("501 Not Implemented"));
    if (req->version != "HTTP/1.1")
        throw (std::runtime_error("505 HTTP Version Not Supported"));
    checkHost(req, &hostname, port);
    if (hostname != conf->host)
        throw (std::runtime_error("Host does not match configuration"));
    if (port != conf->listenPort)
        throw (std::runtime_error("Port does not match configuration"));
    if (!req->body.empty() && req->body.size() > conf->clientMaxBodySize)
        throw (std::runtime_error("413 Payload Too Large"));
}