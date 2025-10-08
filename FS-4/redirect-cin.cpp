#include <iostream>
#include <fcntl.h>
#include <string>
#include <unistd.h>

void initialize(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Argument should be a file path!\n";
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }
    
    if(dup2(fd, 0) == -1) {
        perror("Dup error");
        exit(1);
    }
}

int main(int argc, char** argv) {
    initialize(argc, argv);
    
    std::string input;
    std::cin >> input;
    
    std::string reversed(input.rbegin(), input.rend());
    
    std::cout << reversed << std::endl;
    return 0;
}

