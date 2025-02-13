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

int enable_colors; // ✅ Global değişken burada tanımlandı


#define TASK_CAPACITY 100  // Maximum number of tasks in the queue

static thread_pool_t pool; // Define thread pool globally

int main(int argc, char *argv[])
{
    enable_colors = isatty(STDOUT_FILENO); // Eğer terminalse renkleri aç
  
    if (argc == 2 || (argc > 2 && argv[1][0] != '-')) {
        struct stat statbuf;
        if (fstat(STDIN_FILENO, &statbuf) == 0 && !isatty(STDIN_FILENO)) {
            printf("[INFO] No file provided, reading from stdin...\n");
    
            // Bellek tahsisi
            SearchResultList *results = malloc(sizeof(SearchResultList));
            if (!results) {
                fprintf(stderr, "Memory allocation failed!\n");
                return 1;
            }
            results->head = NULL;
            pthread_mutex_init(&results->mutex, NULL);
    
            // **Arama terimini komut satırından al!**
            if (argc < 2) {
                fprintf(stderr, "[ERROR] No search term provided!\n");
                return 1;
            }
            const char *search_term = argv[1]; // ✅ Arama terimi komut satırından alınıyor
    
            // stdin’den oku ve ara!
            search_stdin(search_term, 0, 0, 0, 0, results);
            free_search_results(results);
            return 0;
        }
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

