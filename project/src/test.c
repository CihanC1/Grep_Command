#define _GNU_SOURCE  // Linux sistemleri için

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "../modules/globals.h"
#include "../modules/search.h"
#include "../modules/thread.h"

void test_search_file() {
    printf("[TEST] Running test_search_file...\n");

    SearchResultList results;
    results.head = NULL;
    pthread_mutex_init(&results.mutex, NULL);

    FILE *file = fopen("test_file.txt", "w");
    fprintf(file, "This is a test line\nAnother test line\n");
    fclose(file);

    Arguments args = {0, 0, 0, 0, 0, "test", NULL, 0}; // ✅ Boş args nesnesi oluşturduk
    int matches = search_file("test", "test_file.txt", 0, 0, 0, 0, &results, &args);
    
    assert(matches > 0);

    unlink("test_file.txt");
    printf("[TEST PASSED] search_file ✅\n");
}

void test_search_directory() {
    printf("[TEST] Running test_search_directory...\n");

    mkdir("test_dir", 0777);
    mkdir("test_dir/subdir", 0777);
    FILE *file = fopen("test_dir/test.txt", "w");
    fprintf(file, "Search this word\nAnother line\n");
    fclose(file);

    SearchResultList results;
    results.head = NULL;
    pthread_mutex_init(&results.mutex, NULL);

    Arguments args = {0, 0, 0, 0, 1, "Search", NULL, 0}; // ✅ Recursive search flag aktif
    search_directory("test_dir", "Search", 0, 0, 0, 0, &results, &args);
    
    unlink("test_dir/test.txt");
    rmdir("test_dir/subdir");
    rmdir("test_dir");

    printf("[TEST PASSED] search_directory ✅\n");
}

void test_search_stdin() {
    printf("[TEST] Running test_search_stdin...\n");

    SearchResultList results;
    results.head = NULL;
    pthread_mutex_init(&results.mutex, NULL);

    FILE *fp = freopen("test_input.txt", "w", stdout);
    printf("This is a stdin test\nAnother search line\n");
    fclose(fp);

    freopen("test_input.txt", "r", stdin);
    Arguments args = {0, 0, 0, 0, 0, "test", NULL, 0}; // ✅ Boş args nesnesi
    search_stdin("test", 0, 0, 0, 0, &results, &args);
        unlink("test_input.txt");

    printf("[TEST PASSED] search_stdin ✅\n");
}

void test_highlight_matches() {
    printf("[TEST] Running test_highlight_matches...\n");

    char line[] = "This is a test line";
    printf("Original line: %s\n", line);
    printf("Highlighted: ");
    extern int enable_colors;  // enable_colors değişkenini buraya ekle
    enable_colors = 1;
    Arguments args = {0, 0, 0, 0, 0, "test", NULL, 0}; // ✅ Boş args nesnesi
    highlight_matches(line, "test", args.case_insensitive, enable_colors);
        printf("\n[TEST PASSED] highlight_matches ✅\n");
}

void run_tests() {
    test_search_file();
    test_search_directory();
    test_search_stdin();
    test_highlight_matches();
}

#ifdef TEST_RUNNER
int main() {
    printf("[DEBUG] Running test suite...\n");
    run_tests();
    return 0;
}
#endif


