#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../modules/search.h"
#include "../modules/thread.h"
#include "../modules/args_parser.h"
#include "../modules/globals.h"

// **TEST 1: `stdin` Üzerinden Arama Testi**
void test_stdin_search() {
    printf("Running: test_stdin_search...\n");

    char *input = "Bu bir test satırıdır.\nKelime burada.\nBaşka bir satır.\n";
    char *search_term = "Kelime";

    char *expected_output = "\033[31mKelime\033[0m burada.\n";  // RENKLİ çıktı bekleniyor

    FILE *stream = fmemopen(input, strlen(input), "r");
    assert(stream != NULL);
    stdin = stream;

    char output_buffer[1024] = {0};
    FILE *output = fmemopen(output_buffer, sizeof(output_buffer), "w");
    assert(output != NULL);
    stdout = output;

    SearchResultList results = {NULL, PTHREAD_MUTEX_INITIALIZER};
    search_stdin(search_term, 0, 0, 0, 0, &results);

    fclose(stream);
    fclose(output);

    assert(strcmp(output_buffer, expected_output) == 0);
    printf("✅ Passed: test_stdin_search\n");
}

// **TEST 2: Dosya Okuma ve Arama Testi**
void test_file_search() {
    printf("Running: test_file_search...\n");

    char *filename = "test_input.txt";
    FILE *test_file = fopen(filename, "w");
    assert(test_file != NULL);
    fprintf(test_file, "Bu bir test satırıdır.\nKelime burada.\nBaşka bir satır.\n");
    fclose(test_file);

    SearchResultList results = {NULL, PTHREAD_MUTEX_INITIALIZER};
    int match_count = search_file("Kelime", filename, 0, 0, 0, 0, &results);

    remove(filename); // Test sonrası dosyayı sil

    assert(match_count == 1);
    printf("✅ Passed: test_file_search\n");
}

// **TEST 3: Thread Havuzu İşleyişi**
void test_thread_pool() {
    printf("Running: test_thread_pool...\n");

    thread_pool_t pool;
    thread_pool_init(&pool, 10, NULL);  // 10 iş parçacıklı thread havuzu başlat

    thread_pool_add_task(&pool, "test.txt", "Kelime");  // Thread havuzuna görev ekle

    thread_pool_destroy(&pool);  // Thread havuzunu temizle

    printf("✅ Passed: test_thread_pool\n");
}

// **TEST 4: Argüman Parsleme Testi**
void test_parse_arguments() {
    printf("Running: test_parse_arguments...\n");

    char *argv[] = {"mygrep", "-i", "-n", "Kelime", "file1.txt", "file2.txt"};
    int argc = 6;

    Arguments args = parse_arguments(argc, argv);

    assert(args.case_insensitive == 1);
    assert(args.show_line_numbers == 1);
    assert(strcmp(args.pattern, "Kelime") == 0);
    assert(args.file_count == 2);
    assert(strcmp(args.files[0], "file1.txt") == 0);
    assert(strcmp(args.files[1], "file2.txt") == 0);

    printf("✅ Passed: test_parse_arguments\n");
}

// **TEST 5: Recursive Dosya Arama**
void test_recursive_search() {
    printf("Running: test_recursive_search...\n");

    char *test_dir = "test_directory";
    mkdir(test_dir, 0777);

    char filename[256];
    snprintf(filename, sizeof(filename), "%s/test_file.txt", test_dir);
    FILE *test_file = fopen(filename, "w");
    assert(test_file != NULL);
    fprintf(test_file, "Bu bir test satırıdır.\nRecursive arama çalışıyor mu?\n");
    fclose(test_file);

    SearchResultList results = {NULL, PTHREAD_MUTEX_INITIALIZER};
    search_directory(test_dir, "Recursive", 0, 0, 0, 0, &results);

    remove(filename);
    rmdir(test_dir);

    printf("✅ Passed: test_recursive_search\n");
}

// **TÜM TESTLERİ ÇALIŞTIR**
void run_all_tests() {
    test_stdin_search();
    test_file_search();
    test_thread_pool();
    test_parse_arguments();
    test_recursive_search();
}

int main() {
    printf("🔍 Running all tests...\n");
    run_all_tests();
    printf("🎉 All tests passed!\n");
    return 0;
}

