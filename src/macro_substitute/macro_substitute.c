#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "macro_substitute.h"

#define MAX_LINE_LEN 1024
#define MAX_IDENT_LEN 128
#define MAX_ITERATIONS 10

/* ---------- Helper functions ---------- */

static int is_ident_start(char c) {
    return isalpha(c) || c == '_';
}

static int is_ident_char(char c) {
    return isalnum(c) || c == '_';
}

/* Search macro by name in the macro table */
static Macro *find_macro(Tabla_macros *tabla, const char *name) {
    if (!tabla) return NULL;

    for (int i = 0; i < tabla->elementos; i++) {
        if (strcmp(tabla->macros[i]->nombre, name) == 0) {
            return tabla->macros[i];
        }
    }
    return NULL;
}

/* ---------- Main substitution function ---------- */

void substitute_macros(char *line, Tabla_macros *tabla_macros) {
    char buffer[MAX_LINE_LEN];
    int iteration = 0;
    int changed;

    if (!line || !tabla_macros) return;

    do {
        int i = 0, j = 0;
        changed = 0;

        while (line[i] != '\0' && j < MAX_LINE_LEN - 1) {

            /* Detect identifier */
            if (is_ident_start(line[i])) {
                char ident[MAX_IDENT_LEN];
                int k = 0;
                int start = i;

                while (is_ident_char(line[i]) && k < MAX_IDENT_LEN - 1) {
                    ident[k++] = line[i++];
                }
                ident[k] = '\0';

                Macro *m = find_macro(tabla_macros, ident);

                /* Substitute only macros without parameters (P1) */
                if (m && m->num_parametros == 0 && m->cuerpo) {
                    int v = 0;
                    while (m->cuerpo[v] && j < MAX_LINE_LEN - 1) {
                        buffer[j++] = m->cuerpo[v++];
                    }
                    changed = 1;
                    continue;
                }

                /* Not a macro: copy original identifier */
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
