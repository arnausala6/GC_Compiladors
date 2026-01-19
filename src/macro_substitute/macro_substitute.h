#ifndef MACRO_SUBSTITUTE_H
#define MACRO_SUBSTITUTE_H

#include <stdio.h>
#include "../module_macrostoring/macrostoring.h"

/**
 * Reads characters from input file and performs macro substitution.
 * Writes the result to the output file.
 */
void macro_substitute(FILE *input, FILE *output, Tabla_macros *tabla_macros);

#endif
