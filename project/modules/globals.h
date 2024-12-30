#ifndef GLOBALS_H
#define GLOBALS_H
#include <pthread.h>

#define MAX_FILES 10
#define BUFFER_SIZE 1024
extern pthread_mutex_t print_mutex;

extern int thread_count;
typedef struct {
    const char *search_term;
    const char *filename;
    int ignore_case;
    int invert_match;
    int show_line_numbers;
} ThreadData;

#endif

