#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/module_macrostoring/macrostoring.h"
#include "../src/macro_substitute/macro_substitute.h"

int main(void) {

    /* 1. Inicializar tabla de macros */
    Tabla_macros tabla;
    tabla.elementos = 0;
    tabla.macros = NULL;

    /* 2. Definir macros (sin parámetros, P1) */
    guardar_macro(&tabla, "A", 0, NULL, "B");
    guardar_macro(&tabla, "B", 0, NULL, "10");

    /* 3. Línea de entrada */
    char line[1024] = "int x = A + 1;";

    printf("Before: %s\n", line);

    /* 4. Sustituir macros */
    substitute_macros(line, &tabla);

    /* 5. Resultado */
    printf("After : %s\n", line);

    return 0;
}
