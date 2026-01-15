#include <ctype.h>
#include <string.h>
#include "macro_substitute.h"
#include "macro_store.h"   // SOLO interfaz, no implementación

#define MAX_LINE_LEN 1024
#define MAX_ITERATIONS 10

static int is_ident_start(char c) {
    return isalpha(c) || c == '_';
}

static int is_ident_char(char c) {
    return isalnum(c) || c == '_';
}

void substitute_macros(char *line) {
    char buffer[MAX_LINE_LEN];
    int iteration = 0;
    int changed;

    do {
        int i = 0, j = 0;
        changed = 0;

        while (line[i] != '\0' && j < MAX_LINE_LEN - 1) {

            if (is_ident_start(line[i])) {
                char ident[128];
                int k = 0;
                int start = i;

                while (is_ident_char(line[i]) && k < 127) {
                    ident[k++] = line[i++];
                }
                ident[k] = '\0';

                if (macro_exists(ident)) {
                    const char *value = macro_get_value(ident);
                    if (value) {
                        for (int v = 0; value[v] && j < MAX_LINE_LEN - 1; v++) {
                            buffer[j++] = value[v];
                        }
                        changed = 1;
                        continue;
                    }
                }

                for (int x = start; x < i && j < MAX_LINE_LEN - 1; x++) {
                    buffer[j++] = line[x];
                }
            } else {
                buffer[j++] = line[i++];
            }
        }

        buffer[j] = '\0';
        strcpy(line, buffer);
        iteration++;

    } while (changed && iteration < MAX_ITERATIONS);
}
