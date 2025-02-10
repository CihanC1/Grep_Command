#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../modules/search.h"
#include "../modules/globals.h"

#define MAX_THREADS 100  

#define RED "\033[31m"
#define RESET "\033[0m"


void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

#pragma GCC diagnostic ignored "-Wnonnull-compare"

char *strcasestr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return NULL;
    size_t len_h = strlen(haystack), len_n = strlen(needle);
    for (size_t i = 0; i <= len_h - len_n; i++) {
        if (strncasecmp(&haystack[i], needle, len_n) == 0) {
            return (char *)&haystack[i];
        }
    }
    return NULL;
}


void add_search_result(SearchResultList *list, const char *filename, int line_number, const char *line_content) {
    if (!list) return;

    SearchResult *new_result = (SearchResult *)malloc(sizeof(SearchResult));
    if (!new_result) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    new_result->filename = strdup(filename);
    new_result->line_number = line_number;
    new_result->line_content = strdup(line_content);
    new_result->next = NULL;

    pthread_mutex_lock(&list->mutex);
    if (!list->head) {
        list->head = new_result;
    } else {
        SearchResult *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_result;
    }
    pthread_mutex_unlock(&list->mutex);
}


void free_search_results(SearchResultList *list) {
    if (!list) return;

    pthread_mutex_lock(&list->mutex);
    SearchResult *current = list->head;
    while (current) {
        SearchResult *next = current->next;
        free(current->filename);
        free(current->line_content);
        free(current);

        current = next;
    }
    list->head = NULL;
    pthread_mutex_unlock(&list->mutex);

    pthread_mutex_destroy(&list->mutex);
    free(list);
}


void *search_directory_thread(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    if (!args) return NULL;
    
    search_directory(args->dir_name, args->search_term, args->ignore_case, args->invert_match, args->show_line_numbers, args->count_only, args->results);
    
    free(args->dir_name);
    free(args);
    pthread_exit(NULL);
}


void highlight_matches(const char *line, const char *search_term, int ignore_case) {
    char *pos = (char *)line;
    size_t term_len = strlen(search_term);

    while (*pos) {
        char *match = ignore_case ? strcasestr(pos, search_term) : strstr(pos, search_term);
        if (!match) {
            printf("%s", pos);
            break;
        }
        printf("%.*s%s%.*s%s", (int)(match - pos), pos, RED, (int)term_len, match, RESET);
        pos = match + term_len;
    }
}


int search_file(const char *search_term, const char *filename, int ignore_case, int invert_match, int show_line_numbers, int count_only, SearchResultList *results) {
    if (!search_term || !filename || !results) return 0;

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "grep: %s: No such file or directory\n", filename);
        return 0;
    }

    char line[BUFFER_SIZE];
    int line_number = 1, match_count = 0;

    while (fgets(line, sizeof(line), file)) {
        int match = ignore_case ? (strcasestr(line, search_term) != NULL) : (strstr(line, search_term) != NULL);
        if (invert_match) match = !match;

        if (match) {
            match_count++;
            if (!count_only) {
                printf("%s: ", filename);
                if (show_line_numbers) {
                    printf("%d:", line_number);
                }
                highlight_matches(line, search_term, ignore_case);
            }
            add_search_result(results, filename, line_number, line);
        }
        line_number++;
    }

    fclose(file);

    if (count_only) {
        printf("%s: %d\n", filename, match_count);
    }

    return count_only ? match_count : 0;
}


void search_directory(const char *dir_name, const char *search_term, int ignore_case, int invert_match, int show_line_numbers, int count_only, SearchResultList *results) {
    if (!dir_name || !search_term || !results) return;

    struct dirent *entry;
    DIR *dir = opendir(dir_name);
    if (!dir) {
        perror("Fehler: Verzeichnis konnte nicht geöffnet werden");
        return;
    }

    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        struct stat path_stat;
        stat(path, &path_stat);

        if (S_ISLNK(path_stat.st_mode)) {
            continue;
        }

        if (S_ISDIR(path_stat.st_mode)) {
            if (thread_count >= MAX_THREADS) {
                printf("Warnung: Die maximale Anzahl an Threads wurde erreicht, das Verzeichnis '%s' wird nicht durchsucht.\n", path);
                continue;
            }

            ThreadArgs *args = malloc(sizeof(ThreadArgs));
            if (!args) {
                fprintf(stderr, "Fehler: Speicherzuweisung fehlgeschlagen!\n");
                continue;
            }

            args->dir_name = strdup(path);
            args->search_term = (char *)search_term;
            args->ignore_case = ignore_case;
            args->invert_match = invert_match;
            args->show_line_numbers = show_line_numbers;
            args->count_only = count_only;
            args->results = results;

            pthread_create(&threads[thread_count], NULL, search_directory_thread, (void *)args);
            thread_count++;
        } else {
            search_file(search_term, path, ignore_case, invert_match, show_line_numbers, count_only, results);
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    closedir(dir);
}
