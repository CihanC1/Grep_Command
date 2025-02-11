#ifndef ARGS_PARSER_H

#define ARGS_PARSER_H

#include <stdbool.h>


typedef struct {
    int case_insensitive; // -i
    int invert_match;     // -v
    int count_matches;    // -c
    int show_line_numbers; // -n
    char *pattern;   
    bool recursive;      
    char **files;         
    int file_count;        
} Arguments;

Arguments parse_arguments(int argc, char *argv[]);

#endif
