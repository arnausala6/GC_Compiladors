#ifndef MACRO_SUBSTITUTE_H
#define MACRO_SUBSTITUTE_H

#include <stdio.h>
#include "macrostoring.h"

/*
 * Intenta sustituir un identificador usando la tabla de macros.
 * 
 * input:
 *   - ident: identificador ya leído (buffer)
 *   - tabla: tabla global de macros
 *   - output: fichero de salida
 *
 * return:
 *   - 1 si se ha hecho sustitución
 *   - 0 si no se ha sustituido
 */
int sustituir_macro(
    const char *ident,
    const Tabla_macros *tabla,
    FILE *output
);

#endif
