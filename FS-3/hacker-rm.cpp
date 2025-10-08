#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Argument should be a file path!\n";
        exit(1);
    }

    int fd = open(argv[1], O_WRONLY);
    if(fd == -1) {
        perror("Open error");
        exit(1);
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if(size == -1) {
        perror("Lseek error");
        close(fd);
        exit(1);
    }

    if(lseek(fd, 0, SEEK_SET) == -1) {
        perror("Lseek error");
        close(fd);
        exit(1);
    }

    size_t BUFFER_SIZE = 4096;
    std::vector<char> buffer(BUFFER_SIZE, 0);

    off_t total_written = 0;
    while(total_written < size) {
        size_t to_write = BUFFER_SIZE;
        if(size - total_written < BUFFER_SIZE) {
            to_write = size - total_written;
        }

        ssize_t written = write(fd, buffer.data(), to_write);
        if(written == -1) {
            perror("Write error");
            close(fd);
            return 1;
        }
        total_written += written;
    }

    close(fd);

    if(unlink(argv[1]) == -1) {
        perror("Delete error");
        exit(1);
    }

    return 0;
}
