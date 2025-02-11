#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../modules/args_parser.h"

Arguments parse_arguments(int argc, char *argv[]) {
    Arguments args = {0, 0, 0, 0, NULL, NULL, 0, 0}; 

    if (argc < 3) {
        fprintf(stderr, "Usage: grep [OPTIONS] SEARCH TERM FILE...\n");
        exit(EXIT_FAILURE);
    }

    args.files = malloc((argc - 1) * sizeof(char *));
    if (!args.files) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    args.file_count = 0;

    for (int i = 1; i < argc; i++) { 

        if (strcmp(argv[i], "-i") == 0) {
    		args.case_insensitive = 1;
        } 
        else if (strcmp(argv[i], "-v") == 0) {
            args.invert_match = 1;
        }
         else if (strcmp(argv[i], "-c") == 0) {
            args.count_matches = 1;
        }
         else if (strcmp(argv[i], "-n") == 0) {
            args.show_line_numbers = 1;
        } 
        else if (strcmp(argv[i], "-r") == 0) {  
            args.recursive = 1;
          
        }  else if (args.pattern == NULL) {
                args.pattern = argv[i]; 
         } else {
                args.files[args.file_count++] = argv[i];  
            }
            
    }
   
      #ifdef DEBUG
printf("Args: case_insensitive=%d, invert_match=%d, count_matches=%d, show_line_numbers=%d, recursive=%d\n",
       args.case_insensitive, args.invert_match, args.count_matches, args.show_line_numbers, args.recursive);
#endif

if (args.pattern == NULL || args.file_count == 0) {

        fprintf(stderr, "Error: At least one file and one search term must be specified!\n");
        exit(EXIT_FAILURE);
    }

    return args;
}
