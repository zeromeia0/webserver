#include "_check.hpp"

static bool isDirective(const std::string &token)
{
	return (token == "server" || token == "listen" || token == "host" || token == "server_name" ||
			token == "client_max_body_size" || token == "error_page" || token == "location" ||
			token == "root" || token == "index" || token == "autoindex" || token == "allowed_methods" ||
			token == "upload_enabled" || token == "upload_store" || token == "redirect"  || token == "alias" || token == "cgi");
}

static bool isNumber(const std::string &token)
{
	if (token.empty())
		return (false);
	for (size_t i = 0; i < token.size(); i++)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[i])))
			return (false);
	}
	return (true);
}

static bool isMethod(const std::string &token)
{
	return (token == "GET" || token == "POST" || token == "DELETE" || token == "HEAD");
}

static size_t findSemicolon(const std::vector<std::string> &tokens, size_t start)
{
	size_t i = start;
	while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}")
		i++;
	if (i >= tokens.size() || tokens[i] != ";")
		THROW("Missing semicolon after directive: " + tokens[start]);
	return (i);
}

static size_t countArgsUntilSemicolon(const std::vector<std::string> &tokens, size_t start)
{
	size_t end = findSemicolon(tokens, start);
	if (end <= start)
		return (0);
	return (end - start - 1);
}

static void expectArgs(const std::vector<std::string> &tokens, size_t i, size_t expected)
{
	size_t count = countArgsUntilSemicolon(tokens, i);
	if (count != expected)
		THROW("Invalid number of arguments in directive: " + tokens[i]);
}

static void checkBraces(const std::vector<std::string> &tokens)
{
	int depth = 0;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "{")
			depth++;
		else if (tokens[i] == "}")
			depth--;
		if (depth < 0)
			THROW("Extra closing brace");
	}
	if (depth != 0)
		THROW("Missing closing brace");
}

static void checkServerBlock(const std::vector<std::string> &tokens)
{
	bool foundServer = false;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "server")
		{
			foundServer = true;
			if (i + 1 >= tokens.size() || tokens[i + 1] != "{")
				THROW("server block must be followed by '{'");
		}
	}
	if (!foundServer)
		THROW("Missing server block");
}

static void checkUnknownDirectives(const std::vector<std::string>& tokens)
{
	for (size_t i = 0; i < tokens.size(); )
	{
		if (tokens[i] == "{" || tokens[i] == "}" || tokens[i] == ";")
		{
			++i;
			continue;
		}
		if (tokens[i] == "server")
		{
			i += 2;
			continue;
		}
		if (tokens[i] == "location")
		{
			i += 3;
			continue;
		}
		if (!isDirective(tokens[i]))
			THROW("Unknown directive: " + tokens[i]);
		++i;
		while (i < tokens.size() && tokens[i] != ";")
			++i;
		if (i < tokens.size())
			++i;
	}
}

static void checkDuplicates(const std::vector<std::string>& tokens)
{
	std::set<std::string> serverDirectives;
	std::set<std::string> locationDirectives;
	std::set<std::string> locationPaths;
	bool inServer = false;
	bool inLocation = false;
	int depth = 0;

	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i] == "server")
		{
			serverDirectives.clear();
			locationPaths.clear();
			inServer = true;
		}
		else if (tokens[i] == "{")
			++depth;
		else if (tokens[i] == "}")
		{
			if (inLocation && depth == 2)
			{
				inLocation = false;
				locationDirectives.clear();
			}
			--depth;
			if (depth == 0)
				inServer = false;
		}
		else if (tokens[i] == "location")
		{
			std::string path = tokens[i + 1];
			if (locationPaths.count(path))
				THROW("Duplicate location: " + path);
			locationPaths.insert(path);
			locationDirectives.clear();
			inLocation = true;
		}
		else if (inServer && !inLocation)
		{
			if (tokens[i] == "host" ||
				tokens[i] == "server_name" ||
				tokens[i] == "client_max_body_size")
			{
				if (serverDirectives.count(tokens[i]))
					THROW("Duplicate directive: " + tokens[i]);
				serverDirectives.insert(tokens[i]);
			}
		}
		else if (inLocation)
		{
			if (tokens[i] == "root" ||
				tokens[i] == "index" ||
				tokens[i] == "autoindex" ||
				tokens[i] == "allowed_methods" ||
				tokens[i] == "upload_enabled" ||
				tokens[i] == "upload_store" ||
				tokens[i] == "redirect" ||
				tokens[i] == "alias")
			{
				if (locationDirectives.count(tokens[i]))
					THROW("Duplicate directive in location: " + tokens[i]);
				locationDirectives.insert(tokens[i]);
			}
		}
	}
}

static void checkDirectiveContext(const std::vector<std::string> &tokens)
{
	int depth = 0;
	bool inLocation = false;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "{")
			depth++;
		else if (tokens[i] == "}")
		{
			if (inLocation && depth == 2)
				inLocation = false;
			depth--;
		}
		else if (tokens[i] == "location")
		{
			if (depth != 1)
				THROW("location directive must be inside server block");
			inLocation = true;
		}
		else if (tokens[i] == "listen" || tokens[i] == "host" || tokens[i] == "server_name" || tokens[i] == "client_max_body_size" || tokens[i] == "error_page")
		{
			if (depth != 1)
				THROW("Server directive in wrong context: " + tokens[i]);
		}
		else if (tokens[i] == "root" || tokens[i] == "index" || tokens[i] == "autoindex" || tokens[i] == "allowed_methods" || tokens[i] == "upload_enabled" || tokens[i] == "upload_store" || tokens[i] == "redirect" || tokens[i] == "alias" || tokens[i] == "cgi")
		{
			if (depth != 2 || !inLocation)
				THROW("Route directive in wrong context: " + tokens[i]);
		}
	}
}

static void checkDirectiveArguments(const std::vector<std::string> &tokens)
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "server")
		{
			if (i + 1 >= tokens.size() || tokens[i + 1] != "{")
				THROW("Invalid server block");
		}
		else if (tokens[i] == "location")
		{
			if (i + 2 >= tokens.size() || tokens[i + 2] != "{")
				THROW("Invalid location block");
			if (tokens[i + 1].empty() || tokens[i + 1][0] != '/')
				THROW("Invalid location path");
		}
		else if (tokens[i] == "listen" || tokens[i] == "host" || tokens[i] == "server_name" || tokens[i] == "client_max_body_size" || tokens[i] == "root" || tokens[i] == "index" || tokens[i] == "autoindex" || tokens[i] == "upload_enabled" || tokens[i] == "upload_store" || tokens[i] == "redirect" || tokens[i] == "alias")
			expectArgs(tokens, i, 1);
		else if (tokens[i] == "error_page")
			expectArgs(tokens, i, 2);
		else if (tokens[i] == "cgi")
			expectArgs(tokens, i, 2);
		else if (tokens[i] == "allowed_methods")
		{
			size_t count = countArgsUntilSemicolon(tokens, i);
			if (count < 1)
				THROW("allowed_methods needs at least one method");
		}
	}
}

static void checkSemicolons(const std::vector<std::string> &tokens)
{
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "server" || tokens[i] == "location" || tokens[i] == "{" || tokens[i] == "}" || tokens[i] == ";")
			continue;
		if (isDirective(tokens[i]))
			findSemicolon(tokens, i);
	}
}

static void checkValues(const std::vector<std::string> &tokens)
{
	bool hasListen = false;
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "listen")
		{
			hasListen = true;
			if (!isNumber(tokens[i + 1]))
				THROW("Invalid listen port: " + tokens[i + 1]);
			int port = std::atoi(tokens[i + 1].c_str());
			if (port < 1 || port > 65535)
				THROW("Listen port out of range");
		}
		else if (tokens[i] == "client_max_body_size")
		{
			if (!isNumber(tokens[i + 1]))
				THROW("Invalid client_max_body_size");
		}
		else if (tokens[i] == "error_page")
		{
			if (!isNumber(tokens[i + 1]))
				THROW("Invalid error code");
			int code = std::atoi(tokens[i + 1].c_str());
			if (code < 100 || code > 599)
				THROW("Error code out of range");
		}
		else if (tokens[i] == "autoindex")
		{
			if (tokens[i + 1] != "on" && tokens[i + 1] != "off")
				THROW("autoindex must be on or off");
		}
		else if (tokens[i] == "upload_enabled")
		{
			if (tokens[i + 1] != "on" && tokens[i + 1] != "off")
				THROW("upload_enabled must be on or off");
		}
		else if (tokens[i] == "allowed_methods")
		{
			size_t j = i + 1;
			while (j < tokens.size() && tokens[j] != ";")
			{
				if (!isMethod(tokens[j]))
					THROW("Invalid HTTP method: " + tokens[j]);
				j++;
			}
		}
		else if (tokens[i] == "location")
		{
			if (tokens[i + 1].empty())
				THROW("Empty location path");
			if (tokens[i + 1][0] != '/')
				THROW("Location path must start with '/'");
		}
		else if (tokens[i] == "cgi")
		{
			if (tokens[i + 1].empty() || tokens[i + 2].empty())
				THROW("Invalid CGI declaration");
			if (tokens[i + 1][0] != '.')
				THROW("CGI extension must start with '.'");
		}
	}
	if (!hasListen)
		THROW("Missing listen directive");
}

void validateSyntax(const std::vector<std::string> &tokens)
{
	if (tokens.empty())
		THROW("Empty configuration file");
	checkBraces(tokens);
	checkUnknownDirectives(tokens);
	checkServerBlock(tokens);
	checkDuplicates(tokens);
	checkSemicolons(tokens);
	checkDirectiveContext(tokens);
	checkDirectiveArguments(tokens);
	checkValues(tokens);
}