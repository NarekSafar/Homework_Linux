#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "A file path should be passed as an argument!\n";
        return 1;
    }

    const char* filePath = argv[1];

    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    const size_t bufferSize = 4096;
    char buffer[bufferSize];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, bufferSize)) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    if (bytesRead < 0) {
        perror("Read error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}

