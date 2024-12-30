#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "search.h"
#include "globals.h"


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


void search_file(const char *search_term, const char *filename, int ignore_case, int invert_match, int show_line_numbers) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        return;
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
            pthread_mutex_lock(&print_mutex); 
            if (show_line_numbers) {
                printf("%s:%d: ", filename, line_number);
            } else {
                printf("%s: ", filename);
            }
            printf("%s", line);
            pthread_mutex_unlock(&print_mutex);
            match_count++;
        }
        line_number++;
    }

    fclose(file);

    pthread_mutex_lock(&print_mutex);  
    printf("File '%s': %d match(es) found.\n", filename, match_count);
    pthread_mutex_unlock(&print_mutex); 
}

