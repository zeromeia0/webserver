#include "_parse.hpp"

static void confAssignValue(sConfigs *server, int &currentRouteIdx, const std::vector<std::string> &tokens, size_t i)
{
	if (tokens[i] == "listen")
		server->listenPorts.push_back(atoi(tokens[i + 1].c_str()));
	else if (tokens[i] == "host")
		server->host = tokens[i + 1];
	else if (tokens[i] == "server_name")
		server->serverName = tokens[i + 1];
	else if (tokens[i] == "client_max_body_size" && currentRouteIdx < 0)
		server->clientMaxBodySize = atoi(tokens[i + 1].c_str());
	else if (tokens[i] == "error_page")
		server->errorPages[atoi(tokens[i + 1].c_str())] = tokens[i + 2];
	else if (tokens[i] == "location")
	{
		sRoute route;
		route.path = tokens[i + 1];
		server->router.push_back(route);
		currentRouteIdx = server->router.size() - 1;
	}
	else if (currentRouteIdx >= 0)
	{
		sRoute *currentRoute = &server->router[currentRouteIdx];
		if (tokens[i] == "root")
			currentRoute->root = tokens[i + 1];
		else if (tokens[i] == "index")
			currentRoute->index = tokens[i + 1];
		else if (tokens[i] == "autoindex")
			currentRoute->autoindex = (tokens[i + 1] == "on");
		else if (tokens[i] == "upload_enabled")
			currentRoute->uploadEnabled = (tokens[i + 1] == "on");
		else if (tokens[i] == "upload_store")
			currentRoute->uploadStore = tokens[i + 1];
		else if (tokens[i] == "redirect")
			currentRoute->redirect = tokens[i + 1];
		else if (tokens[i] == "alias")
			currentRoute->alias = tokens[i + 1];
		else if (tokens[i] == "client_max_body_size")
			currentRoute->clientMaxBodySize = atoi(tokens[i + 1].c_str());
		else if (tokens[i] == "allowed_methods")
		{
			size_t j = i + 1;
			while (j < tokens.size() && tokens[j] != ";")
			{
				currentRoute->methods.push_back(tokens[j]);
				j++;
			}
		}
		else if (tokens[i] == "cgi")
			currentRoute->cgi[tokens[i + 1]] = tokens[i + 2];
	}
}

static std::string validateFile(char *fileName) {
	int fd = open(fileName, O_RDONLY);
	if (fd < 0)
		THROW("Config file can not be opened");
	char buffer[2048];
	std::string fileContent;
	int bytes;
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
		fileContent.append(buffer, bytes);
	close(fd);
	return (fileContent);
}

std::vector<sConfigs*> parseConfigs(char *fileName) {
	std::vector<sConfigs*> CONFS;
	std::string file = validateFile(fileName);
	std::vector<std::string> tokens = tokenize(file);
	validateSyntax(tokens);
	int currentRouteIdx = -1;
	for (size_t i = 0; i < tokens.size(); i++) {
		if (tokens[i] == "server") {
			CONFS.push_back(new sConfigs);
			currentRouteIdx = -1;
			continue;
		}
		if (CONFS.empty())
			continue;
		confAssignValue(CONFS.back(), currentRouteIdx, tokens, i);
	}
	return (CONFS);
}
