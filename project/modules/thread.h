#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#define MAX_THREADS 8

typedef struct {
    char *filename;
    char *search_term;
} thread_data_t;

typedef struct {
    thread_data_t *tasks;
    pthread_t threads[MAX_THREADS];
    int task_count;
    int task_capacity;
    int shutdown;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Arguments *args;
} thread_pool_t;

void thread_pool_init(thread_pool_t *pool, int capacity, Arguments *args);
void thread_pool_add_task(thread_pool_t *pool, const char *filename, const char *search_term);
void thread_pool_destroy(thread_pool_t *pool);
void *worker_thread(void *arg);

#endif // THREAD_H

