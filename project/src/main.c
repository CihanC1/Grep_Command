#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"
#include "thread.h"

static thread_pool_t pool; // Eksik pool değişkeni tanımlandı

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s [-i] [-v] [-n] [-c] [-r] <search_term> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    Arguments args = parse_arguments(argc, argv);
    thread_pool_init(&pool);

    SearchResultList *results = malloc(sizeof(SearchResultList));
    if (!results)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    results->head = NULL;
    pthread_mutex_init(&results->mutex, NULL);

    for (int i = 0; i < args.file_count; i++)
    {
        struct stat path_stat;
        stat(args.files[i], &path_stat);

        if (S_ISDIR(path_stat.st_mode))
        {
            if (args.recursive)
            {
                search_directory(args.files[i], args.pattern, args.case_insensitive, args.invert_match, args.show_line_numbers, args.count_matches, results);
            }
            else
            {
                fprintf(stderr, "Error: %s is a directory, but -r flag was not provided.\n", args.files[i]);
            }
        }
        else
        {
            add_task(&pool, args.files[i], args.pattern);
        }
    }

    thread_pool_destroy(&pool);
    free_search_results(results);
    free(args.files);
    return 0;
}
