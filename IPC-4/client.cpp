#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 54000
#define BUFFER_SIZE 1024

int sock;

void* receive_messages(void*) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes <= 0)
            break;
        std::cout << buffer;
    }
    exit(0);
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));

    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    name += "\n";
    send(sock, name.c_str(), name.size(), 0);

    pthread_t tid;
    pthread_create(&tid, nullptr, receive_messages, nullptr);

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        message += "\n";
        send(sock, message.c_str(), message.size(), 0);

        if (message == "/exit\n")
            break;
    }

    close(sock);
    return 0;
}
