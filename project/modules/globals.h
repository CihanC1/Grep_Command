#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#define MAX_GLOBAL_THREADS 100  

#define MAX_FILES 10
#define BUFFER_SIZE 1024

extern pthread_mutex_t print_mutex;
extern int enable_colors;


typedef struct SearchResult {
    char *filename;
    int line_number;
    char *line_content;
    struct SearchResult *next;
} SearchResult;


typedef struct {
    SearchResult *head;
    pthread_mutex_t mutex;
} SearchResultList;


typedef struct {
    const char *search_term;
    const char *filename;
    int ignore_case;
    int invert_match;
    int show_line_numbers;
    int count_only;  
    SearchResultList *results;
} ThreadData;


typedef struct {
    char *dir_name;
    char *search_term;
    int ignore_case;
    int invert_match;
    int show_line_numbers;
    int count_only;
    SearchResultList *results;
} ThreadArgs;

#endif 
