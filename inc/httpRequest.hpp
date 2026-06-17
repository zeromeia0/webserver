/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   httpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 15:35:56 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/17 08:37:55 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Server;
class httpRequest
{
    public: 
        struct request
        {
            std::string method;
            std::string uri;
            std::string version;
            std::map<std::string, std::string> headers;
            std::string body;
        };
        httpRequest() {};
        ~httpRequest() {};
        void httpRequestDebbuger(httpRequest::request *req, int clientFd);
};

#include "webserver.hpp"