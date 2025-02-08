#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"

void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    search_file(data->search_term, data->filename, data->ignore_case, data->invert_match, data->show_line_numbers, data->count_only, data->results);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-i] [-v] [-n] [-c] [-r] <search_term> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    Arguments args = parse_arguments(argc, argv);

    // **Debug Çıktısı**
           args.case_insensitive, args.invert_match, args.count_matches, args.show_line_numbers, args.recursive;

    // **THREAD-SAFE BELLEK TAHSİSİ**
    SearchResultList *results = malloc(sizeof(SearchResultList));
    if (!results) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    results->head = NULL;
    pthread_mutex_init(&results->mutex, NULL);

    pthread_t threads[MAX_FILES];
    ThreadData *thread_data = malloc(args.file_count * sizeof(ThreadData));
    if (!thread_data) {
        fprintf(stderr, "ThreadData memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < args.file_count; i++) {
        struct stat path_stat;
        stat(args.files[i], &path_stat);

        if (S_ISDIR(path_stat.st_mode)) {
            if (args.recursive) {
                search_directory(args.files[i], args.pattern, args.case_insensitive, args.invert_match, args.show_line_numbers, args.count_matches, results);
            } else {
                fprintf(stderr, "Fehler: %s ist ein Verzeichnis, aber das -r-Flag wurde nicht angegeben.\n", args.files[i]);
            }
        } else {
            thread_data[i].search_term = args.pattern;
            thread_data[i].filename = args.files[i];
            thread_data[i].ignore_case = args.case_insensitive;
            thread_data[i].invert_match = args.invert_match;
            thread_data[i].show_line_numbers = args.show_line_numbers;
            thread_data[i].count_only = args.count_matches;
            thread_data[i].results = results;

            pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
        }
    }

    for (int i = 0; i < args.file_count; i++) {
        pthread_join(threads[i], NULL);
    }

  
    for (int i = 0; i < args.file_count; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // **BELLEK TEMİZLEME (NULL KONTROLLERİ EKLENDİ!)**
    free_search_results(results);
    free(thread_data);
    free(args.files);
    
}
