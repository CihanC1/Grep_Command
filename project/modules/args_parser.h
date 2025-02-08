#ifndef ARGS_PARSER_H

#define ARGS_PARSER_H

#include <stdbool.h>


typedef struct {
    bool case_insensitive; // -i
    bool invert_match;     // -v
    bool count_matches;    // -c
    bool show_line_numbers; // -n
    char *pattern;   
    bool recursive;      
    char **files;         
    int file_count;        
} Arguments;

Arguments parse_arguments(int argc, char *argv[]);

#endif
