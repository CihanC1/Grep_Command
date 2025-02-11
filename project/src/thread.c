#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"
#include "../modules/thread.h"


void *worker_thread(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;
    
    while (1) {
        pthread_mutex_lock(&pool->mutex);

        while (pool->task_count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->cond, &pool->mutex);
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutex);
            return NULL;
        }

        pool->task_count--;
        thread_data_t task = pool->tasks[pool->task_count];

        pthread_mutex_unlock(&pool->mutex);

        // 🔹 `args`'i `pool` içinden al
        Arguments *args = pool->args;

        SearchResultList results;
        results.head = NULL;
        pthread_mutex_init(&results.mutex, NULL);

/*        printf("[DEBUG] search_file çağrılıyor: ignore_case = %d\n", args->case_insensitive);
        fflush(stdout);
*/
        search_file(task.search_term, task.filename, args->case_insensitive, args->invert_match, args->show_line_numbers, args->count_matches, &results);

        free(task.filename);
        free(task.search_term);
    }
}

/* Initialize the thread pool */
void thread_pool_init(thread_pool_t *pool, int capacity, Arguments *args) {
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->task_count = 0;
    pool->shutdown = 0;
    pool->task_capacity = capacity;
    pool->args = args;  // ✅ `args`'i pool içinde sakla

    pool->tasks = malloc(sizeof(thread_data_t) * pool->task_capacity);
    if (!pool->tasks) {
        fprintf(stderr, "[ERROR] Memory allocation for tasks failed!\n");
        exit(1);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, (void *)pool) != 0) {
            fprintf(stderr, "[ERROR] Failed to create worker thread %d\n", i);
        }
    }
}

/* Add a task to the pool */
void thread_pool_add_task(thread_pool_t *pool, const char *filename, const char *search_term) {
    if (!pool || !filename || !search_term) {
        printf("[ERROR] Invalid task parameters!\n");
        return;
    }

    pthread_mutex_lock(&pool->mutex);
/*
    printf("[DEBUG] Adding task: Searching '%s' in '%s'...\n", search_term, filename);
*/
    if (pool->task_count < pool->task_capacity) {
        pool->tasks[pool->task_count].filename = strdup(filename);
        pool->tasks[pool->task_count].search_term = strdup(search_term);
        pool->task_count++;
  /*      printf("[DEBUG] Task added! Current task count: %d\n", pool->task_count);
    */
      	  pthread_cond_signal(&pool->cond);  // THREAD'İ UYANDIR!
    } else {
        printf("[ERROR] Task queue is full!\n");
    }

    pthread_mutex_unlock(&pool->mutex);
}
/* Destroy the thread pool */
void thread_pool_destroy(thread_pool_t *pool) {
    pthread_mutex_lock(&pool->mutex);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->tasks);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}

