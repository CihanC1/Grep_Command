#ifndef SEARCH_H
#define SEARCH_H

#include "globals.h"

int search_file(const char *search_term, const char *filename, int ignore_case, int invert_match, int show_line_numbers, int count_only, SearchResultList *results);

void *search_directory_thread(void *arg);
void highlight_matches(const char *line, const char *search_term, int ignore_case, int enable_colors);


void search_directory(const char *dir_name, const char *search_term, int ignore_case, int invert_match, int show_line_numbers, int count_only, SearchResultList *results);

void add_search_result(SearchResultList *list, const char *filename, int line_number, const char *line_content);

void free_search_results(SearchResultList *list);


void search_stdin(const char *search_term, int ignore_case, int invert_match, int show_line_numbers, int count_only, SearchResultList *results);


#endif
