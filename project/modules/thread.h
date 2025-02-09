/* thread.h - Thread management for parallel grep */
#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "../modules/globals.h"

#define MAX_THREADS 8 // Maximum number of threads in the pool

typedef struct
{
    char *filename;
    char *search_term;
    int ignore_case;
    int invert_match;
    int show_line_numbers;
    int count_only;
    SearchResultList *results;
} thread_data_t;

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    thread_data_t tasks[MAX_THREADS];
    int task_count;
} thread_pool_t;

void thread_pool_init(thread_pool_t *pool);
void thread_pool_destroy(thread_pool_t *pool);
void add_task(thread_pool_t *pool, char *filename, char *search_term);
void *worker_thread(void *arg);

#endif // THREAD_H