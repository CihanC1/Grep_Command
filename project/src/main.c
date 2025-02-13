#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/args_parser.h"
#include "../modules/thread.h"

#define TASK_KAPAZITÄT 100

static thread_pool_t pool;

int main(int argc, char *argv[])
{
    enable_colors = isatty(STDOUT_FILENO);

    if (argc == 2 || (argc > 2 && argv[1][0] != '-'))
    {
        struct stat statbuf;
        if (fstat(STDIN_FILENO, &statbuf) == 0 && !isatty(STDIN_FILENO))
        {
            printf("[INFO] Keine Datei angegeben, lese aus der Standardeingabe...\n");

            SearchResultList *results = malloc(sizeof(SearchResultList));
            if (!results)
            {
                fprintf(stderr, "Fehler: Speicherzuweisung fehlgeschlagen!\n");
                return 1;
            }
            results->head = NULL;
            pthread_mutex_init(&results->mutex, NULL);

            if (argc < 2)
            {
                fprintf(stderr, "[FEHLER] Kein Suchbegriff angegeben!\n");
                return 1;
            }
            const char *search_term = argv[1];

            search_stdin(search_term, 0, 0, 0, 0, results);
            free_search_results(results);
            return 0;
        }
    }

    Arguments args = parse_arguments(argc, argv);

#ifdef DEBUG
    printf("Argumente: Groß-/Kleinschreibung ignorieren=%d, invertierte Übereinstimmung=%d, Anzahl der Übereinstimmungen=%d, Zeilennummern anzeigen=%d, rekursiv=%d\n",
           args.case_insensitive, args.invert_match, args.count_matches, args.show_line_numbers, args.recursive);
#endif

    thread_pool_init(&pool, TASK_KAPAZITÄT, &args);

    SearchResultList *results = malloc(sizeof(SearchResultList));
    if (!results)
    {
        fprintf(stderr, "Fehler: Speicherzuweisung fehlgeschlagen!\n");
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
                search_directory(args.files[i], args.pattern, args.case_insensitive, args.invert_match,
                                 args.show_line_numbers, args.count_matches, results);
            }
            else
            {
                fprintf(stderr, "Fehler: %s ist ein Verzeichnis, aber das -r-Flag wurde nicht angegeben.\n", args.files[i]);
            }
        }
        else
        {
            thread_pool_add_task(&pool, args.files[i], args.pattern);
        }
    }

    sleep(1);

    thread_pool_destroy(&pool);
    free_search_results(results);
    free(args.files);

    return 0;
}
