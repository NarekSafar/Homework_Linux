#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 80
#define BUFFER_SIZE 4096

int main() {
    const char* hostname = "httpforever.com";

    hostent* host = gethostbyname(hostname);
    if (!host) {
        std::cerr << "gethostbyname failed\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "socket failed\n";
        return 1;
    }

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "connect failed\n";
        close(sock);
        return 1;
    }

    const char* request =
        "GET / HTTP/1.1\r\n"
        "Host: httpforever.com\r\n"
        "Connection: close\r\n\r\n";

    send(sock, request, strlen(request), 0);

    int file_fd = open("httpforever.html", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) {
        std::cerr << "file open failed\n";
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    while ((bytes = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        write(file_fd, buffer, bytes);
    }

    close(file_fd);
    close(sock);

    std::cout << "Response saved to httpforever.html\n";
    return 0;
}
