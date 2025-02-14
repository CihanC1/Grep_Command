#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "args_parser.h"  // ✅ Arguments yapısını tanımlamak için ekledik

#define MAX_THREADS 8

typedef struct {
    char *filename;
    char *search_term;
} thread_data_t;

typedef struct {
    pthread_t *threads;  // Thread dizisini tanımla
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    thread_data_t *tasks;
    int task_count;
    int thread_count;
    int task_capacity;
    int shutdown;
    Arguments *args;
} thread_pool_t;

void thread_pool_init(thread_pool_t *pool, int capacity, Arguments *args);
void thread_pool_add_task(thread_pool_t *pool, const char *filename, const char *search_term);
void thread_pool_destroy(thread_pool_t *pool);
void *worker_thread(void *arg);

#endif // THREAD_H
