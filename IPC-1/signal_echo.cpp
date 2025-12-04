#include <iostream>
#include <signal.h>
#include <pwd.h>

void handler(int signum, siginfo_t* info, void* context) {
	ucontext_t* cxt = (ucontext_t*)(context);
	pid_t sender_pid = info->si_pid;
	uid_t sender_uid = info->si_uid;

	const char* username = "";
	struct passwd* pw = getpwuid(sender_uid);
	if(pw) username = pw->pw_name;

	std::cout << "Received a SIGUSR1 signal from process " << sender_pid << " executed by " << sender_uid
		<< " (" << username << ").\n";
	std::cout << "State of the context: EIP = " << std::hex << (cxt->uc_mcontext.gregs[REG_RIP] & 0xFFFFFFFF)
		<< ", EAX = " << std::hex << (cxt->uc_mcontext.gregs[REG_RAX] & 0xFFFFFFFF) << ", EBX = " <<
		std::hex << (cxt->uc_mcontext.gregs[REG_RBX] & 0xFFFFFFFF)  << ".\n";
}

int main() {
	struct sigaction sa{};
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;

	if(sigaction(SIGUSR1, &sa, nullptr) == -1) {
		std::cerr << "sigaction error\n";
		exit(1);
	}

	std::cout << "PID: " << getpid() << '\n';

	while(true) {
		std::cout << "Program is running...\n";
		sleep(3);
	}

	return 0;
}
