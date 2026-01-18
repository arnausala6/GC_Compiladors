#ifndef MACRO_SUBSTITUTE_H
#define MACRO_SUBSTITUTE_H

#include "../module_macrostoring/macrostoring.h"

/**
 * Performs macro substitution on a line of code.
 * - line: input/output line to be modified
 * - tabla_macros: table containing all defined macros
 */
void substitute_macros(char *line, Tabla_macros *tabla_macros);

#endif
