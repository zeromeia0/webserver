#include "_parse.hpp"

std::vector<std::string> tokenize(const std::string& file)
{
	std::vector<std::string> tokens;
	std::string current;

	for (size_t i = 0; i < file.size(); i++)
	{
		char c = file[i];
		if (std::isspace(c))
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
		}
		else if (c == '{' || c == '}' || c == ';')
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
			tokens.push_back(std::string(1, c));
		}
		else
			current += c;
	}
	if (!current.empty())
		tokens.push_back(current);
	return (tokens);
}
