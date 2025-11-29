#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

void sample_task(void* arg) {
    int value = *static_cast<int*>(arg);

    std::cout << "Executing task " << value
              << " in thread " << pthread_self()
              << std::endl;

    usleep(200000);
}

int main() {
    parallel_scheduler pool(3); 

    int args[10];
    for (int i = 0; i < 10; i++) {
        args[i] = i;
        pool.run(sample_task, &args[i]);
    }

    sleep(3); 
    return 0;
}
