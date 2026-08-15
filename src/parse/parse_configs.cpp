#include "_parse.hpp"

static void saveConfigs(sConfigs *server) {
	std::string filename = "./var/data/configs.json";
	std::remove(filename.c_str());
	std::string content;
	content.append("{");
	content.append("\"host\": \"" + server->host + "\"");
	content.append(", \"server name\": \"" + server->serverName + "\"");
	content.append(", \"client max body size\": \"" + intToChar(server->clientMaxBodySize) + "\"");
	content.append(", \"ports\": " + vectorToListString<int>(server->listenPorts));
	content.append(", \"routes\": ");
	content.append("[");
	for (size_t i = 0; i < server->router.size(); i++) {
		content.append("{");
		content.append("\"path\": \"" + server->router[i].path + "\"");
		content.append(", \"root\": \"" + server->router[i].root + "\"");
		content.append(", \"index\": \"" + server->router[i].index + "\"");
		content.append(", \"uploadStore\": \"" + server->router[i].uploadStore + "\"");
		content.append(", \"redirect\": \"" + server->router[i].redirect + "\"");
		content.append(", \"autoindex\": \"" + intToChar(server->router[i].autoindex) + "\"");
		content.append(", \"uploadEnabled\": \"" + (server->router[i].uploadEnabled ? std::string("TRUE") : std::string("FALSE")) + "\"");
		content.append(", \"methods\": " + vectorToListString<std::string>(server->router[i].methods));
		content.append(", \"cgi\": " + mapToJsonString<std::string, std::string>(server->router[i].cgi));
		content.append("}");
		if (i + 1 != server->router.size())
			content.append(", ");
	}
	content.append("]");
	content.append("}");
	writeFileContent(filename, content);
}

static void confAssignValue(sConfigs *server, sRoute *&currentRoute, const std::vector<std::string> &tokens, size_t i)
{
    if (tokens[i] == "listen")
        server->listenPorts.push_back(atoi(tokens[i + 1].c_str()));
    else if (tokens[i] == "host")
        server->host = tokens[i + 1];
    else if (tokens[i] == "server_name")
        server->serverName = tokens[i + 1];
    else if (tokens[i] == "client_max_body_size")
        server->clientMaxBodySize = atoi(tokens[i + 1].c_str());
    else if (tokens[i] == "error_page")
        server->errorPages[atoi(tokens[i + 1].c_str())] = tokens[i + 2];
    else if (tokens[i] == "location")
    {
        sRoute route;
        route.path = tokens[i + 1];
        server->router.push_back(route);
        currentRoute = &server->router.back();
    }
    else if (currentRoute)
    {
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

static void resetConfig(sConfigs *CONF) {
	CONF->listenPorts.clear();
	CONF->host.clear();
	CONF->serverName.clear();
	CONF->clientMaxBodySize = 0;
	CONF->errorPages.clear();
	CONF->router.clear();
	CONF->confFile.clear();
}

static std::string validateFile(char *fileName)
{
    int fd = open(fileName, O_RDONLY);
    if (fd < 0)
        throw (std::runtime_error("Can't open file"));
    char buffer[2048];
    std::string fileContent;
    int bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
        fileContent.append(buffer, bytes);
    close(fd);
    return (fileContent);
}

sConfigs *parseConfigs(char *fileName) {
	sConfigs *CONF = new sConfigs;
	resetConfig(CONF);
	std::string file = validateFile(fileName);
	CONF->confFile = tokenize(file);
	for (size_t i = 0; i < CONF->confFile.size(); i++)
		LOG("DEBUG", CONF->confFile[i]);
	validateSyntax(CONF->confFile);
	sRoute *currentRoute = NULL;
	for (size_t i = 0; i < CONF->confFile.size(); i++)
		confAssignValue(CONF, currentRoute, CONF->confFile, i);
	saveConfigs(CONF);
	return (CONF);
}
