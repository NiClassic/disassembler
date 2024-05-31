//
// Created by niclassic on 09.05.24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction_types.h"

#define INPUT_SIZE 16

void decode_and_print(const char *input) {
    uint32_t number = (uint32_t) strtol(input, NULL, 0);
    const char *instruction = to_string(decode_instruction(number));
    if (instruction == NULL) {
        printf("Error while parsing instruction.\n");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", instruction);
}

int main(int argc, char **argv) {
    //Repl mode
    if (argc == 1) {
        printf("Enter an instruction (e.g. 0x80000537). Exit with \"exit\".\n");
        while (1) {
            printf(">> ");
            char line[INPUT_SIZE];
            if (fgets(line, sizeof(line), stdin)) {
                if (strcmp(line, "exit\n") == 0) {
                    exit(EXIT_SUCCESS);
                }
                decode_and_print(line);
            } else {
                fprintf(stderr, "Failed to read input.\n");
                exit(EXIT_FAILURE);
            }
        }
    } else if (argc != 2) {
        printf("Must specify file.\n");
        exit(EXIT_FAILURE);
    }
    //File Mode
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Could not read file.\n");
        exit(EXIT_FAILURE);
    }
    char content[INPUT_SIZE];
    while (fgets(content, sizeof(content), file)) {
        decode_and_print(content);
    }

    fclose(file);
    return EXIT_SUCCESS;
}
