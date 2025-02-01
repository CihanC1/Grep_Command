#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "search.h"
#include "../modules/globals.h"


void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}


int contains(const char *line, const char *term, int ignore_case) {
    char line_copy[BUFFER_SIZE], term_copy[BUFFER_SIZE];
    if (ignore_case) {
        strncpy(line_copy, line, sizeof(line_copy));
        strncpy(term_copy, term, sizeof(term_copy));
        to_lowercase(line_copy);
        to_lowercase(term_copy);
        return strstr(line_copy, term_copy) != NULL;
    }
    return strstr(line, term) != NULL;
}


int search_file(const char *search_term, const char *filename, int ignore_case, int invert_match, int show_line_numbers, int count_only) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "grep: %s: No such file or directory\n", filename);
        return 0;
    }

    char line[BUFFER_SIZE];
    int line_number = 1;
    int match_count = 0;

    while (fgets(line, sizeof(line), file)) {
        int match = contains(line, search_term, ignore_case);
        if (invert_match) {
            match = !match;
        }

        if (match) {
            if (!count_only) {  
                pthread_mutex_lock(&print_mutex);
                if (show_line_numbers) {
                    printf("%s:%d:%s", filename, line_number, line);
                } else {
                    printf("%s: %s", filename, line);
                }
                pthread_mutex_unlock(&print_mutex);
            }
            match_count++;  
        }
        line_number++;
    }

    fclose(file);

    if (count_only) {
        return match_count;  
    }

    pthread_mutex_lock(&print_mutex);
    printf("\nFile '%s': %d match(es) found.\n", filename, match_count);
    pthread_mutex_unlock(&print_mutex);

    return 0;  
}