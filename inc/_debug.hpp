#pragma once

#include "main.hpp"
#include RE_HPP
#include <map>
#include <vector>

void		debugRe(Re &re);
void		debugRes(Response &res);
void		debugReq(Request &req);
void		debugConfigs(sConfigs *configs);

template	<typename T>
void		debugVector(std::vector<T> vec) {
	for (size_t i = 0; i < vec.size(); i++)
		std::cerr << vec[i] << std::endl;
}

template	<typename T, typename U>
void		debugMap(std::map<T, U> map) {
	for (typename std::map<T, U>::iterator it = map.begin(); it != map.end(); ++it) {
		std::cerr << it->first << " -> \"" << it->second << "\"" << std::endl;
	}
}
