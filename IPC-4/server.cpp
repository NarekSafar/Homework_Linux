#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 54000
#define BUFFER_SIZE 1024

struct Client {
    int socket;
    std::string name;
};

std::vector<Client> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const std::string& message, int exclude_socket = -1) {
    pthread_mutex_lock(&clients_mutex);
    for (const auto& c : clients) {
        if (c.socket != exclude_socket) {
            send(c.socket, message.c_str(), message.size(), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_user_list(int client_socket) {
    pthread_mutex_lock(&clients_mutex);
    std::string list = "Users online:\n";
    for (const auto& c : clients) {
        list += "- " + c.name + "\n";
    }
    pthread_mutex_unlock(&clients_mutex);
    send(client_socket, list.c_str(), list.size(), 0);
}

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    delete (int*)arg;

    char buffer[BUFFER_SIZE];
    std::string name;

    recv(client_socket, buffer, BUFFER_SIZE, 0);
    name = buffer;

    pthread_mutex_lock(&clients_mutex);
    clients.push_back({client_socket, name});
    pthread_mutex_unlock(&clients_mutex);

    broadcast(name + " joined the chat\n");

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes <= 0)
            break;

        std::string msg(buffer);

        if (msg == "/exit\n") {
            break;
        }
        else if (msg == "/list\n") {
            send_user_list(client_socket);
        }
        else {
            broadcast(name + ": " + msg, client_socket);
        }
    }

    pthread_mutex_lock(&clients_mutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->socket == client_socket) {
            clients.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    broadcast(name + " left the chat\n");
    close(client_socket);
    pthread_exit(nullptr);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    std::cout << "Server started on port " << PORT << std::endl;

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);

        pthread_t tid;
        int* pclient = new int(client_socket);
        pthread_create(&tid, nullptr, handle_client, pclient);
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}
