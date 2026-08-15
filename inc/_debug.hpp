#pragma once

#include "#MAIN.hpp"
#include RE_HPP

void		debugRe(Re &re, bool printPayload);
void		debugRes(Response &res);
void		debugReq(Request &req);
void		confDebbuger(sConfigs *conf);

template	<typename T>
void		printVector(std::vector<T> vec) {
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << vec[i] << std::endl;
}

template	<typename T, typename U>
void		printMap(std::map<T, U> map) {
	for (typename std::map<T, U>::iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " -> " << it->second << std::endl;
	}
}
