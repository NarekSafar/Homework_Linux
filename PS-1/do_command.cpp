#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <chrono>
#include <cerrno>

void do_command(char** argv) {
	auto start = std::chrono::steady_clock::now();

	pid_t pid = fork();

	if (pid < 0) {
		perror("Fork failed");
		exit(1);
	} else if (pid == 0) {
		execvp(argv[1], argv + 1);
		//perror("Exec failed");
		if (errno == ENOENT) {
			exit(127);
		} else if (errno == EACCES) {
			exit(126);
		} else {
			exit(1);
		}
	} else {
		int status;
		wait(&status);

		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> duration = end - start;

		if (WIFEXITED(status)) {
			int exit_code = WEXITSTATUS(status);
			std::cout << "\nCommand exited with code " << exit_code
			          << " and took " << duration.count() << " seconds.\n";

			if (exit_code == 127) {
				std::cerr << "Error: Command not found.\n";
			} else if (exit_code == 126) {
				std::cerr << "Error: Permission denied or not executable.\n";
			} else if (exit_code != 0) {
				std::cerr << "Error: Command failed with non-zero exit.\n";
			}
		} else {
			std::cerr << "\nCommand terminated abnormally and took "
			          << duration.count() << " seconds.\n";
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Usage: ./do-command <command> [args...]\n";
		exit(1);
	}

	do_command(argv);
	return 0;
}

