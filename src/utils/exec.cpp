#include "_exec.hpp"

sRoute findRoute(const std::string &uri, const std::vector<sRoute> &router) {
	sRoute ret;
	size_t longest = 0;
	for (std::vector<sRoute>::const_iterator it = router.begin(); it != router.end(); ++it ) {
		std::string routerPath = it->path;
		size_t routerPathSize = routerPath.size();
		std::string pathSubstr = uri.substr(0, routerPathSize);
		if (pathSubstr == routerPath && routerPathSize > longest) {
			ret = *it;
			longest = routerPathSize;
		}
	}
	return (ret);
}

std::string autoindex( std::string path, std::string base_path ) {
	LOG("DEBUG", __FUNCTION__);

	std::string output;
	output += "<h2>" + base_path + "</h2>\n";
    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        std::cerr << "Error opening directory" << std::endl;
        return (output);
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
		output += "<div><a href=" + base_path + "/" + entry->d_name + " >" + entry->d_name + "<a></div>\n";
    closedir(dir);
	return (output);
}
