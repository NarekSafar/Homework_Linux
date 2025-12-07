#include <iostream>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>

bool is_prime(unsigned long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;
    unsigned long long r = std::sqrt((long double)n);
    for (unsigned long long d = 3; d <= r; d += 2)
        if (n % d == 0) return false;
    return true;
}

unsigned long long nth_prime(unsigned long long m) {
    unsigned long long c = 0, x = 1;
    while (c < m) {
        x++;
        if (is_prime(x)) c++;
    }
    return x;
}

ssize_t write_all(int fd, const void* buf, size_t n) {
    size_t w = 0;
    const char* p = (const char*)buf;
    while (w < n) {
        ssize_t k = write(fd, p + w, n - w);
        if (k <= 0) return -1;
        w += k;
    }
    return w;
}

ssize_t read_all(int fd, void* buf, size_t n) {
    size_t r = 0;
    char* p = (char*)buf;
    while (r < n) {
        ssize_t k = read(fd, p + r, n - r);
        if (k <= 0) return k;
        r += k;
    }
    return r;
}

int main() {
    int p2c[2], c2p[2];
    pipe(p2c);
    pipe(c2p);

    pid_t pid = fork();
    
    if (pid == 0) {
        close(p2c[1]);
        close(c2p[0]);
        while (true) {
            long long m;
            if (read_all(p2c[0], &m, sizeof(m)) <= 0) break;
            if (m == 0) break;
            std::cout << "[Child] Calculating " << m << "-th prime number...\n";
            unsigned long long ans = nth_prime(m);
            std::cout << "[Child] Sending calculation result of prime(" << m << ")...\n";
            write_all(c2p[1], &ans, sizeof(ans));
        }
        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    }

    close(p2c[0]);
    close(c2p[1]);

    while (true) {
        std::cout << "[Parent] Please enter the number: ";
        std::string s;
        if (!std::getline(std::cin, s)) break;
        if (s == "exit") {
            long long sentinel = 0;
            write_all(p2c[1], &sentinel, sizeof(sentinel));
            break;
        }
        long long m;
        try { m = std::stoll(s); } catch (...) { continue; }
        if (m <= 0) continue;

        std::cout << "[Parent] Sending " << m << " to the child process...\n";
        long long sendm = m;
        write_all(p2c[1], &sendm, sizeof(sendm));

        std::cout << "[Parent] Waiting for the response from the child process...\n";
        unsigned long long result;
        if (read_all(c2p[0], &result, sizeof(result)) <= 0) break;

        std::cout << "[Parent] Received calculation result of prime(" 
                  << m << ") = " << result << "...\n";
    }

    close(p2c[1]);
    close(c2p[0]);
    wait(nullptr);
    return 0;
}
