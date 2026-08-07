/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvazzs <vvazzs@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 12:24:17 by vvazzs            #+#    #+#             */
/*   Updated: 2026/06/12 15:16:20 by vvazzs           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_debug.hpp"

void debugRe(Re re, bool printPayload) {
	std::cout << "version: " << re.headers.version << std::endl;
	std::cout << "path: " << re.headers.path << std::endl;
	std::cout << "headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = re.headers.headers.begin(); it != re.headers.headers.end(); ++it) {
		std::cout << "key: \"" << it->first << "\" value: \"" << it->second << "\"" << std::endl;
	}
	std::cout << "method: " << *getMethodTxt(re.headers.method) << std::endl;
	std::cout << "status_code: " << re.status_code << std::endl;
	if (printPayload)
		std::cout << "payload: " << re.payload << std::endl;
}
