#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_reader.h"

int read_thrust_strings(const char *filename, char *collection[MAX_STRINGS]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    int string_count = 0;
    int buffer_capacity = 1024;
    char *buffer = malloc(buffer_capacity);
    int buffer_index = 0;
    int ch, inside_quotes = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '"') {
            if (!inside_quotes) {
                inside_quotes = 1;
                buffer_index = 0;
            } else {
                inside_quotes = 0;
                buffer[buffer_index] = '\0';
                if (string_count < MAX_STRINGS) {
                    collection[string_count] = strdup(buffer);
                    string_count++;
                }
            }
        } else if (inside_quotes) {
            if (buffer_index >= buffer_capacity - 1) {
                buffer_capacity *= 2;
                buffer = realloc(buffer, buffer_capacity);
            }
            buffer[buffer_index++] = (char)ch;
        }
    }

    fclose(file);
    free(buffer);
    return string_count;
}
