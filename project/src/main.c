#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"


void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    search_file(data->search_term, data->filename, data->ignore_case, data->invert_match, data->show_line_numbers, 0); // 0 = normal mod
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-i] [-v] [-n] [-c] <search_term> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

   
    Arguments args = parse_arguments(argc, argv);

    // -c (count) flag'i kontrolü
    if (args.count_matches) {
        int total_matches = 0;
        for (int i = 0; i < args.file_count; i++) {
            total_matches += search_file(args.pattern, args.files[i], args.case_insensitive, args.invert_match, args.show_line_numbers, 1); // 1 = count mode
        }
        printf("%d\n", total_matches);  
        free(args.files);
        return 0;  
    }


    pthread_t threads[MAX_FILES];
    ThreadData thread_data[MAX_FILES];
    int thread_count = 0;  

    for (int i = 0; i < args.file_count; i++) {
        thread_data[i].search_term = args.pattern;
        thread_data[i].filename = args.files[i];
        thread_data[i].ignore_case = args.case_insensitive;
        thread_data[i].invert_match = args.invert_match;
        thread_data[i].show_line_numbers = args.show_line_numbers;

       
        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0) {
            perror("Thread oluşturulamadı!");
            continue;
        }
        thread_count++; 
    }

    
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed. Total threads executed: %d\n", thread_count);

    free(args.files);
    return 0;
}
