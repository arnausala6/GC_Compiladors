#include "macro_substitute.h"
#include <string.h>

/*
 * Sustitución textual de macros sin parámetros.
 * El identificador ya viene completo desde el Reader.
 */
int sustituir_macro(
    const char *ident,
    const Tabla_macros *tabla,
    FILE *output
) {
    if (!ident || !tabla || !output) {
        return 0;
    }

    for (int i = 0; i < tabla->elementos; i++) {
        Macro *m = tabla->macros[i];

        if (strcmp(ident, m->nombre) == 0) {
            // Macro encontrada → escribir su cuerpo
            fputs(m->cuerpo, output);
            return 1;
        }
    }

    // No es una macro → no se sustituye
    return 0;
}
