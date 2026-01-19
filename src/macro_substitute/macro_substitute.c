#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "macro_substitute.h"

#define MAX_BUFFER 256

/* Busca una macro por nombre */
static Macro *buscar_macro(Tabla_macros *tabla, const char *nombre) {
    for (int i = 0; i < tabla->elementos; i++) {
        if (strcmp(tabla->macros[i]->nombre, nombre) == 0) {
            return tabla->macros[i];
        }
    }
    return NULL;
}

void macro_substitute(FILE *input, FILE *output, Tabla_macros *tabla_macros) {
    char buffer[MAX_BUFFER];
    int buffer_len = 0;
    int c;

    while ((c = fgetc(input)) != EOF) {

        /* Inicio posible de identificador */
        if (isalpha(c) || c == '_') {
            buffer_len = 0;
            buffer[buffer_len++] = (char)c;

            /* Leer el resto del identificador */
            while ((c = fgetc(input)) != EOF && (isalnum(c) || c == '_')) {
                if (buffer_len < MAX_BUFFER - 1) {
                    buffer[buffer_len++] = (char)c;
                }
            }
            buffer[buffer_len] = '\0';

            /* Comprobar si es una macro */
            Macro *m = buscar_macro(tabla_macros, buffer);

            if (m && m->num_parametros == 0) {
                /* Sustituir macro */
                fputs(m->cuerpo, output);
            } else {
                /* No es macro: escribir tal cual */
                fputs(buffer, output);
            }

            /* El último carácter leído no pertenece al identificador */
            if (c != EOF) {
                fputc(c, output);
            }
        } 
        else {
            /* No es inicio de identificador */
            fputc(c, output);
        }
    }
}
