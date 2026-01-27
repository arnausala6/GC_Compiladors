/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: user_manual.c
* Autor: Arnau Sala
* Fecha: 17/01/2026
*
* Descripción:
* Este módulo se encarga de mostrar el manual de usuario del preprocesador.
* Lee el contenido del archivo de texto del manual y lo imprime por la terminal.
*
* Se utiliza cuando el programa se ejecuta con la opción -help.
* -----------------------------------------------------------------------------
*/


#include "user_manual.h"
#include <string.h>

#define MANPAGE_PATH "../../src/user_manual/user_manual.txt"

int print_help(FILE *out) {
    FILE *fp;
    char buffer[512];

    if (!out) return 1;

    fp = fopen(MANPAGE_PATH, "r");
    if (!fp) {
        fprintf(out, "Error loading the user manual.\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fputs(buffer, out);
    }

    fclose(fp);
    return 0;
}