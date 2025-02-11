#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> // for sleep()
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"
#include "../modules/thread.h"

#define TASK_CAPACITY 100  // Maximum number of tasks in the queue

static thread_pool_t pool; // Define thread pool globally

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s [-i] [-v] [-n] [-c] [-r] <search_term> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    // **Parse Command-Line Arguments**
    Arguments args = parse_arguments(argc, argv);

    #ifdef DEBUG
    printf("Args: case_insensitive=%d, invert_match=%d, count_matches=%d, show_line_numbers=%d, recursive=%d\n",
           args.case_insensitive, args.invert_match, args.count_matches, args.show_line_numbers, args.recursive);
    #endif

    // **Initialize the Thread Pool**
    thread_pool_init(&pool, TASK_CAPACITY, &args);  

    // **Allocate Memory for Results (Thread-Safe)**
    SearchResultList *results = malloc(sizeof(SearchResultList));
    if (!results)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    results->head = NULL;
    pthread_mutex_init(&results->mutex, NULL);

    // **Iterate Over Files and Add Tasks**
    for (int i = 0; i < args.file_count; i++)
    {
        struct stat path_stat;
        stat(args.files[i], &path_stat);

        if (S_ISDIR(path_stat.st_mode))
        {
            if (args.recursive)
            {
		    printf("[ONEMLI DEBUG]! Ne zaman giriyor buraya ve burasi neden threadste degil?");
                search_directory(args.files[i], args.pattern, args.case_insensitive, args.invert_match, 
                                 args.show_line_numbers, args.count_matches, results);
            }
            else
            {
                fprintf(stderr, "Error: %s is a directory, but -r flag was not provided.\n", args.files[i]);
            }
        }
        else
        {
            thread_pool_add_task(&pool, args.files[i], args.pattern);
        }
    }

    // **Wait for Tasks to Complete**
    sleep(1);  // Adjust if needed to allow workers to process tasks

    // **Shutdown and Cleanup**
    thread_pool_destroy(&pool);
    free_search_results(results);
    free(args.files);

    return 0;
}

