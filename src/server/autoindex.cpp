#include "../main.hpp"

std::string autoindex( std::string path, std::string base_path) {
	char *py_bin = (char *)"/usr/bin/python3";
	char *args[] = {
		py_bin,
		(char *)"./var/cgi-bin/autoindex.py",
		NULL
	};
	std::string FOLDER = "FOLDER=" + path;
	std::string BASE_PATH = "BASE_PATH=" + base_path;
	char *envp[] = {
		(char *)FOLDER.c_str(),
		(char *)BASE_PATH.c_str(),
		NULL
	};
	return (cgi(py_bin, args, envp));
}
