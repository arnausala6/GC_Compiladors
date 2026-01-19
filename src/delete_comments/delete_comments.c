/*
delete_comments.c
---------------------------------------------------------------------------

Implementación de la lógica de eliminación de comentarios para el pre-procesador P1PP.

Funcionalidad:
    - Elimina comentarios de una línea: //
    - Elimina comentarios de bloque:   / * ... * /
    - Sustituye cada comentario eliminado por un espacio.
    - Mantiene los saltos de línea fuera de los comentarios para que no se colapsen líneas de código distintas.

  Notas:
      - Este módulo no conoce nada de directivas ni macros, sólo de texto.
      - Diseñado como motor independiente para poder probarlo con un main
        de test o integrarlo en el preprocesador general.
---------------------------------------------------------------------------
 */
#include "delete_comments.h"

static void escribir_si_valido(int c, FILE *out) {
    if (c != EOF) fputc(c, out);
}

static void consumir_comentario_linea(FILE *in, FILE *out, int *linea_actual) {
    int c;
    while ((c = fgetc(in)) != EOF) {
        if (c == '\n') {
            escribir_si_valido('\n', out);
            if (linea_actual) (*linea_actual)++;
            return;
        }
    }
}

static void consumir_comentario_bloque(FILE *in, FILE *out, int *linea_actual) {
    int c;
    int visto_asterisco = 0;

    while ((c = fgetc(in)) != EOF) {
        if (c == '\n') {
            escribir_si_valido('\n', out);
            if (linea_actual) (*linea_actual)++;
            visto_asterisco = 0;
            continue;
        }

        if (visto_asterisco && c == '/') {
            return; // fin del bloque
        }

        visto_asterisco = (c == '*');
    }
}

dc_result_t manejar_comentario(FILE *in, FILE *out, int *linea_actual) {
    int tipo;

    if (!in || !out) return DC_ERROR_PARAM;

    // Sustituimos TODO comentario por un espacio
    escribir_si_valido(' ', out);

    // Leemos el segundo carácter: '/' o '*'
    tipo = fgetc(in);
    if (tipo == EOF) return DC_OK;

    if (tipo == '/') {
        consumir_comentario_linea(in, out, linea_actual);
    } else if (tipo == '*') {
        consumir_comentario_bloque(in, out, linea_actual);
    } else {
        // Caso raro: el motor se equivocó. Devolvemos lo consumido como texto "seguro".
        // (Aquí no reinsertamos el '/', porque el motor ya lo consumió; por contrato no debería ocurrir)
        escribir_si_valido(tipo, out);
    }

    return DC_OK;
}

dc_result_t procesar_fichero_sin_comentarios(FILE *in, FILE *out, int *linea_actual) {
    int c, siguiente;

    if (!in || !out) return DC_ERROR_PARAM;
    if (linea_actual && *linea_actual <= 0) *linea_actual = 1;

    while ((c = fgetc(in)) != EOF) {
        if (c == '/') {
            siguiente = fgetc(in);
            if (siguiente == '/' || siguiente == '*') {
                ungetc(siguiente, in);      // dejamos el tipo
                manejar_comentario(in, out, linea_actual);
            } else {
                escribir_si_valido('/', out);
                if (siguiente != EOF) ungetc(siguiente, in);
            }
        } else {
            escribir_si_valido(c, out);
            if (c == '\n' && linea_actual) (*linea_actual)++;
        }
    }

    return DC_OK;
}