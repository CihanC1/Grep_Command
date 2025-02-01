#ifndef SEARCH_H
#include "globals.h"

#define SEARCH_H
//change to int. for -c flag 
int search_file(const char *search_term, const char *filename, int ignore_case, int invert_match, int show_line_numbers, int count_only);


#endif

