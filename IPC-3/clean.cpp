#include <sys/mman.h>
#include <semaphore.h>
#include <iostream>

int main() {
    shm_unlink("/shared_nums");
    sem_unlink("/shared_lock");
    std::cout << "resources removed\n";
}
