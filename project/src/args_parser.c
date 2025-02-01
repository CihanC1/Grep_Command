#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../modules/args_parser.h"

Arguments parse_arguments(int argc, char *argv[]) {

    Arguments args = {false, false, false, false, NULL, NULL, 0};

    if (argc < 3) {
        fprintf(stderr, "Kullanim: grep [OPTIONS] PATTERN FILE...\n");
        exit(EXIT_FAILURE);
    }

    args.files = malloc((argc - 1) * sizeof(char *));

    args.file_count = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            args.case_insensitive = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            args.invert_match = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            args.count_matches = true;
        } else if (strcmp(argv[i], "-r") == 0) {
            args.recursive = true;
        } else if (args.pattern == NULL) {
            args.pattern = argv[i];
        } else {
            args.files[args.file_count++] = argv[i];
        }
    }

    if (args.pattern == NULL || args.file_count == 0) {
        fprintf(stderr, "Error: One pattern and at least one file must be specified!\n");
        exit(EXIT_FAILURE);
    }

    return args;
}
