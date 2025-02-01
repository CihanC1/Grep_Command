#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <stdbool.h>

typedef struct {
    bool case_insensitive; // -i
    bool invert_match;     // -v
    bool count_matches;    // -c
    bool recursive;        // -r
    bool show_line_numbers; // -n 
    char *pattern;         // Aranacak kelime
    char **files;          // Dosya listesi
    int file_count;        // Kaç dosyada arama yapılacak
} Arguments;

Arguments parse_arguments(int argc, char *argv[]);


#endif
