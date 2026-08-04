#include "Re.hpp"
#include "../../_main.hpp"

std::string *cgi(const char *file, char **args, char **envp) {
	int pipefds[2];
	pipe(pipefds);
	pid_t pid = fork();
	if (pid == 0) {
		close(pipefds[0]);
		dup2(pipefds[1], STDOUT_FILENO);
		close(pipefds[1]);
		execve(file, args, envp);
		exit(1);
	}
	close(pipefds[1]);
	char buf[4096];
	std::string *output = new std::string;
	int bytes;
	while ((bytes = read(pipefds[0], buf, sizeof(buf))) > 0)
		output->append(buf, bytes);
	close(pipefds[0]);
	int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		return (NULL);
	}
	return (output);
}
