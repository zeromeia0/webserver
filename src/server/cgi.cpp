#include "../main.hpp"

std::string cgi(const char *file, char **args, char **envp) {
	int pipefds[2];
	pipe(pipefds);
	pid_t pid = fork();
	LOG("PID", pid);
	if (pid == 0) {
		close(pipefds[0]);
		dup2(pipefds[1], STDOUT_FILENO);
		close(pipefds[1]);
		execve(file, args, envp);
		exit(1);
	}
	close(pipefds[1]);
	char buf[4096];
	std::string output;
	int bytes;
	while ((bytes = read(pipefds[0], buf, sizeof(buf))) > 0)
		output.append(buf, bytes);
	close(pipefds[0]);
	waitpid(pid, NULL, 0);
	return (output);
}
