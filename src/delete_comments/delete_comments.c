/*
delete_comments.c
---------------------------------------------------------------------------
Implementación de eliminación de comentarios para P1PP.

- Elimina comentarios: // y / * ... * /
- Sustituye cada comentario por un espacio
- Mantiene saltos de línea para no colapsar el código
---------------------------------------------------------------------------
*/

#include "delete_comments.h"

// comprueba si se puede escribir
static void escribir_si_valido(int c, FILE *out) {
    if (c != EOF) {
        fputc(c, out);
    }
}

// para borrar comentarios // 
static void consumir_comentario_linea(FILE *in, FILE *out, int *linea_actual) {
    int c;
    while ((c = fgetc(in)) != EOF) {
        if (c == '\n') {
            escribir_si_valido('\n', out);
            if (linea_actual) {
                (*linea_actual)++;
            }
            return;
        }
    }
}


// para borrar comentarios /* */
static void consumir_comentario_bloque(FILE *in, FILE *out, int *linea_actual) {
    int c;
    int asterisco_visto = 0;

    while ((c = fgetc(in)) != EOF) {
        if (c == '\n') {
            escribir_si_valido('\n', out);
            if (linea_actual) {
                (*linea_actual)++;
            }
            asterisco_visto = 0;
            continue;
        }

        if (asterisco_visto && c == '/') {
            return; // fin del bloque
        }

        asterisco_visto = (c == '*');
    }
}


// handler
void manejar_comentario(FILE *in, FILE *out, int *linea_actual) {
    
    int tipo;

    if (!in || !out) {
        return;
    }

    // Sustituimos el comentario por un espacio (una sola vez)
    escribir_si_valido(' ', out);

    // El motor dejó el tipo ('/' o '*') en el stream
    tipo = fgetc(in);
    if (tipo == EOF) {
        return;
    }

    if (tipo == '/') {
        consumir_comentario_linea(in, out, linea_actual);
    } 
    else if (tipo == '*') {
        consumir_comentario_bloque(in, out, linea_actual);
    } 
    else {
        escribir_si_valido(tipo, out);
    }
}

int procesar_fichero_sin_comentarios(FILE *in, FILE *out, int *linea_actual) {
    int c, siguiente;

    if (!in || !out) {
        return 1;
    }
    if (linea_actual && *linea_actual <= 0) {
        *linea_actual = 1;
    }

    while ((c = fgetc(in)) != EOF) {
        if (c == '/') {
            siguiente = fgetc(in);
            if (siguiente == '/' || siguiente == '*') {
                ungetc(siguiente, in); // dejamos el tipo
                manejar_comentario(in, out, linea_actual);
            } 
            else {
                escribir_si_valido('/', out);
                if (siguiente != EOF) {
                    ungetc(siguiente, in);
                }
            }
        } 
        else {
            escribir_si_valido(c, out);
            if (c == '\n' && linea_actual) {
                (*linea_actual)++;
            }
        }
    }

    return 0;
}