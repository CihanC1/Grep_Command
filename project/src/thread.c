/* thread.c - Thread management for parallel grep */

#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"

static thread_pool_t pool;
static pthread_t threads[MAX_THREADS];

void *worker_thread(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    search_file(data->search_term, data->filename, data->ignore_case, data->invert_match, data->show_line_numbers, data->count_only, data->results);
    printf("[THREAD] %s içinde '%s' aranıyor...\n", data->filename, data->search_term);
    return NULL;
}

void thread_pool_init(thread_pool_t *pool)
{
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->task_count = 0;
    for (int i = 0; i < MAX_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, worker_thread, NULL);
    }
}

void thread_pool_destroy(thread_pool_t *pool)
{
    for (int i = 0; i < MAX_THREADS; i++)
    {
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}

void add_task(thread_pool_t *pool, char *filename, char *search_term)
{
    pthread_mutex_lock(&pool->mutex);
    if (pool->task_count < MAX_THREADS)
    {
        pool->tasks[pool->task_count].filename = strdup(filename);
        pool->tasks[pool->task_count].search_term = strdup(search_term);
        pool->task_count++;
        pthread_cond_signal(&pool->cond);
    }
    pthread_mutex_unlock(&pool->mutex);
}

void search_directory(const char *dirpath, const char *search_term)
{
    struct dirent *entry;
    struct stat path_stat;
    char fullpath[1024];

    DIR *dir = opendir(dirpath);
    if (!dir)
    {
        perror("Dizin açılamadı");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);
        stat(fullpath, &path_stat);

        if (S_ISDIR(path_stat.st_mode))
        {
            // Eğer bir alt dizin bulduysak, içine girip tekrar ara
            search_directory(fullpath, search_term);
        }
        else
        {
            // Dosyayı thread havuzuna ekle
            add_task(&pool, fullpath, search_term);
        }
    }
    closedir(dir);
}
