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

std::string *cgi(const char *file, char **args, char **envp, const std::string &payload) {
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

		execve(file, args, envp);
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
	return (*cgi(py_bin, args, envp, ""));
}