#include "shared_array.h"
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    shared_array numbers("/shared_nums", 10);

    sem_t* sync = sem_open("/shared_lock", O_CREAT, 0666, 1);
    if (sync == SEM_FAILED) {
        std::cerr << "Semaphore open failed\n";
        return 1;
    }

    while (true) {
        sem_wait(sync);
        numbers[0] += 2;
        std::cout << "[first] value = " << numbers[0] << '\n';
        sem_post(sync);
        sleep(1);
    }
}
