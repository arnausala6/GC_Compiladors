#include <stdio.h>
#include <stdlib.h>

#include "../src/module_macrostoring/macrostoring.h"
#include "../src/macro_substitute/macro_substitute.h"

int main(void) {

    /* ---------- Crear tabla de macros ---------- */
    Tabla_macros tabla;
    tabla.elementos = 0;
    tabla.macros = NULL;

    /* ---------- Guardar macros (P1: sin parámetros) ---------- */
    guardar_macro(&tabla, "A", 0, NULL, "B");
    guardar_macro(&tabla, "B", 0, NULL, "10");

    /* ---------- Abrir ficheros ---------- */
    FILE *input = fopen("input_test.c", "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output = fopen("output_test.c", "w");
    if (!output) {
        perror("Error opening output file");
        fclose(input);
        return 1;
    }

    /* ---------- Ejecutar sustitución de macros ---------- */
    macro_substitute(input, output, &tabla);

    /* ---------- Cerrar ficheros ---------- */
    fclose(input);
    fclose(output);

    printf("Macro substitution test finished.\n");
    printf("Check output_test.c for results.\n");

    return 0;
}
