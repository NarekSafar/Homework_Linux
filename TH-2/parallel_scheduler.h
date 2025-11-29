#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <pthread.h>
#include <queue>
#include <vector>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(void (*func)(void*), void* arg);

private:
    struct Task {
        void (*func)(void*);
        void* arg;
    };

    static void* worker_entry(void* arg);
    void worker_loop();

    size_t capacity;
    std::vector<pthread_t> threads;

    std::queue<Task> task_queue;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cond;

    bool stop;
};

#endif
