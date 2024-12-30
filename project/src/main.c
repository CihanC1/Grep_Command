#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "search.h"
#include "globals.h" 

void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    search_file(data->search_term, data->filename, data->ignore_case, data->invert_match, data->show_line_numbers);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-i] [-v] [-n] <search_term> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    
    int ignore_case = 0, invert_match = 0, show_line_numbers = 0;
    const char *search_term = NULL;
    const char *files[MAX_FILES];
    int file_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            invert_match = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
        } else if (!search_term) {
            search_term = argv[i];
        } else if (file_count < MAX_FILES) {
            files[file_count++] = argv[i];
        }
    }

    if (!search_term || file_count == 0) {
        printf("Usage: %s [-i] [-v] [-n] <search_term> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    pthread_t threads[MAX_FILES];
    ThreadData thread_data[MAX_FILES];


    thread_count = 0;


    for (int i = 0; i < file_count; i++) {
        thread_data[i].search_term = search_term;
        thread_data[i].filename = files[i];
        thread_data[i].ignore_case = ignore_case;
        thread_data[i].invert_match = invert_match;
        thread_data[i].show_line_numbers = show_line_numbers;

        pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
        thread_count++; 
    }


    for (int i = 0; i < file_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed. Total threads executed: %d\n", thread_count);

    return 0;
}

