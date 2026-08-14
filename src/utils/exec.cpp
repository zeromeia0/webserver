#include "_exec.hpp"

serverRoute findRoute(std::string uri, std::vector<serverRoute> router) {
	serverRoute ret;
	size_t longest = 0;
	for (std::vector<serverRoute>::const_iterator it = router.begin(); it != router.end(); ++it ) {
		std::string routerPath = it->path;
		size_t routerPathSize = routerPath.size();
		std::string pathSubstr = uri.substr(0, routerPathSize);
		if (pathSubstr == routerPath && routerPathSize > longest)
			ret = *it;
	}
	return (ret);
}

std::string *cgi(char *bin, char *file, std::map<std::string, std::string> inputs, std::string &payload) {
	LOG("DEBUG", __FUNCTION__);
	int pipe_out[2];
	pipe(pipe_out);

	int pipe_in[2];
	pipe(pipe_in);

	pid_t pid = fork();

	if (pid == 0) {

		close(pipe_out[0]);
		close(pipe_in[1]);

		dup2(pipe_in[0], STDIN_FILENO);
		close(pipe_in[0]);

		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_out[1]);

		char *args[] = { bin, file, NULL };

		// Build envp — strings must outlive execve
		std::vector<std::string> env_strs;
		for (std::map<std::string, std::string>::iterator it = inputs.begin(); it != inputs.end(); ++it)
			env_strs.push_back(it->first + "=" + it->second);

		char **envp = new char*[env_strs.size() + 1];
		for (size_t i = 0; i < env_strs.size(); i++)
			envp[i] = (char *)env_strs[i].c_str();
		envp[env_strs.size()] = NULL;

		execve(bin, args, envp);
		exit(1);
	}

	close(pipe_out[1]);
	close(pipe_in[0]);

	if (!payload.empty())
		write(pipe_in[1], payload.c_str(), payload.size());
	
	close(pipe_in[1]);

	char buf[4096];
	std::string *output = new std::string;
	int bytes;
	while ((bytes = read(pipe_out[0], buf, sizeof(buf))) > 0)
		output->append(buf, bytes);

	close(pipe_out[0]);

	int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		delete output;
		return (NULL);
	}
	return (output);
}

std::string autoindex( std::string path, std::string base_path ) {
	char *bin = (char *)"/usr/bin/python3";
	char *file = (char *)"./var/cgi-bin/autoindex.py";
	std::map<std::string, std::string> inputs;
	inputs.insert(std::pair<std::string, std::string>("FOLDER", path));
	inputs.insert(std::pair<std::string, std::string>("BASE_PATH", base_path));
	std::string payload = "";
	return (*cgi(bin, file, inputs, payload));
}