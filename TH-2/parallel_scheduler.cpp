#include "parallel_scheduler.h"
#include <iostream>

parallel_scheduler::parallel_scheduler(size_t capacity)
    : capacity(capacity), stop(false) 
{
    pthread_mutex_init(&queue_mutex, nullptr);
    pthread_cond_init(&queue_cond, nullptr);

    threads.resize(capacity);
    for (size_t i = 0; i < capacity; ++i) {
        pthread_create(&threads[i], nullptr, worker_entry, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&queue_mutex);
    stop = true;
    pthread_cond_broadcast(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);

    for (pthread_t& t : threads) {
        pthread_join(t, nullptr);
    }

    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_cond);
}

void parallel_scheduler::run(void (*func)(void*), void* arg) {
    pthread_mutex_lock(&queue_mutex);
    task_queue.push({func, arg});
    pthread_cond_signal(&queue_cond);
    pthread_mutex_unlock(&queue_mutex);
}

void* parallel_scheduler::worker_entry(void* arg) {
    parallel_scheduler* scheduler = static_cast<parallel_scheduler*>(arg);
    scheduler->worker_loop();
    return nullptr;
}

void parallel_scheduler::worker_loop() {
    while (true) {
        pthread_mutex_lock(&queue_mutex);

        while (!stop && task_queue.empty()) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }

        if (stop && task_queue.empty()) {
            pthread_mutex_unlock(&queue_mutex);
            return;
        }

        Task task = task_queue.front();
        task_queue.pop();

        pthread_mutex_unlock(&queue_mutex);

        task.func(task.arg);
    }
}
