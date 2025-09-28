#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Two file paths should be passed as arguments!\n";
        return 1;
    }

    const char* sourcePath = argv[1];
    const char* destPath = argv[2];

    int srcFd = open(sourcePath, O_RDONLY);
    if (srcFd == -1) {
        perror("Failed to open source file");
        exit(EXIT_FAILURE);
    }

    int destFd = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        perror("Failed to open or create destination file");
        close(srcFd);
        exit(EXIT_FAILURE);
    }

    const size_t bufferSize = 4096;
    char buffer[bufferSize];
    ssize_t bytesRead;

    while ((bytesRead = read(srcFd, buffer, bufferSize)) > 0) {
        ssize_t bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            std::cerr << "Write error!\n";
            close(srcFd);
            close(destFd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytesRead < 0) {
        perror("Read error");
        close(srcFd);
        close(destFd);
        exit(EXIT_FAILURE);
    }

    close(srcFd);
    close(destFd);
    return 0;
}

