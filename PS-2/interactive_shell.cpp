#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

std::vector<std::string> split(const std::string& str, const std::string& delim) {
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delim);

	while (end < str.size()) {
		std::string token = str.substr(start, end - start);
		if (!token.empty()) tokens.push_back(token);
		start = end + delim.size();
		end = str.find(delim, start);
	}

	std::string token = str.substr(start);
	if (!token.empty()) tokens.push_back(token);

	return tokens;
}

int executeCommand(const std::string& cmd) {
	std::istringstream iss(cmd);
	std::vector<std::string> args;
	std::string word;
	while (iss >> word) {
		args.push_back(word);
	}

	if (args.empty()) return 0;

	bool isSilent = false;
	if (args[0] == "silent") {
		isSilent = true;
		args.erase(args.begin());
	}

	std::string redirectionSymbol = "";
	bool isOutputRedirected = false;
	std::string outputFileName;

	if(args.size() > 2) {
		if (args[args.size()- 2] == ">" || args[args.size() - 2] == ">>") {
			outputFileName = args.back();
			redirectionSymbol = args[args.size()- 2];
			isOutputRedirected = true;
			args.erase(args.end() - 2, args.end());
		}
	}

	if (args.empty()) {
		std::cerr << "<silent> is not a command.\n";
		return 1;
	}

	// if (args[0] == "exit") return 0;
	if (isSilent && isOutputRedirected) {
		std::cerr << "<silent> cannot be used with '>' or '>>' operators.\n";
		return 1;
	}

	std::vector<char*> argv(args.size() + 1);
	for (size_t i = 0; i < args.size(); ++i) {
		argv[i] = const_cast<char*>(args[i].c_str());
	}
	argv[args.size()] = nullptr;

	pid_t pid = fork();

	if (pid == 0) {
		if (isSilent) {
			std::string filename = std::to_string(getpid()) + ".log";
			int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1) {
				perror("open error");
				exit(1);
			}
			if (dup2(fd, 1) == -1 || dup2(fd, 2) == -1) {
				perror("dup error");
				close(fd);
				exit(1);
			}
			close(fd);
		}

		if (isOutputRedirected) {
			int fd;
			if(redirectionSymbol == ">") {
				fd = open(outputFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			} else {
				fd = open(outputFileName.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
			if (fd == -1) {
				perror("open error");
				exit(1);
			}
			if (dup2(fd, 1) == -1) {
				perror("dup error");
				close(fd);
				exit(1);
			}
			close(fd);
		}

		char* old_path = getenv("PATH");
		std::string new_path = std::string("PATH=.") +
		                       (old_path ? (":" + std::string(old_path)) : "");
		setenv("PATH", new_path.c_str(), 1);

		execvp(argv[0], argv.data());
		perror("execvp failed");
		exit(1);
	} else if (pid > 0) {
		int status;
		waitpid(pid, &status, 0);
		return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
	} else {
		perror("fork failed");
		return 1;
	}
}

int main() {
	std::string input;
	while (true) {
		std::cout << "$ ";
		std::getline(std::cin, input);

		if (input.empty()) continue;
		if (input == "exit") break;

		std::vector<std::string> semicolonParts = split(input, ";");

		for (const std::string& part : semicolonParts) {

			std::vector<std::string> andParts = split(part, "&&");

			bool isFirstCommandDone = true;

			for (const std::string& orPart : andParts) {
				std::vector<std::string> orParts = split(orPart, "||");
				if (!isFirstCommandDone) {
					orParts.erase(orParts.begin());
				}
				auto it = orParts.begin();
				for(it = orParts.begin(); it != orParts.end(); ++it) {
					int exit_code = executeCommand(*it);
					if (exit_code == 0) {
						isFirstCommandDone = true;
						break;
					}
				}
				if(it == orParts.end()) {
					isFirstCommandDone = false;
				}
			}
		}
	}
	return 0;
}

