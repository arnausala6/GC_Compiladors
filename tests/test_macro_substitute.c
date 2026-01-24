#include <stdio.h>
#include <stdlib.h>

#include "macrostoring.h"
#include "macro_substitute.h"

int main(void) {
    // 1. Crear tabla de macros vacía
    Tabla_macros tabla;
    tabla.elementos = 0;
    tabla.macros = NULL;

    // 2. Añadir macros
    guardar_macro(&tabla, "PI", 0, NULL, "3.14");
    guardar_macro(&tabla, "HELLO", 0, NULL, "HolaMundo");

    // 3. Abrir fichero de salida temporal
    FILE *output = fopen("test_output.txt", "w");
    if (!output) {
        perror("fopen");
        return 1;
    }

    // 4. Identificadores de prueba
    const char *tests[] = {
        "PI",
        "x",
        "HELLO",
        "y"
    };

    printf("Ejecutando test de macro_substitute...\n");

    for (int i = 0; i < 4; i++) {
        printf("Entrada: %s -> Salida: ", tests[i]);

        if (!sustituir_macro(tests[i], &tabla, output)) {
            // No es macro, se escribe tal cual
            fputs(tests[i], output);
            printf("%s\n", tests[i]);
        } else {
            printf("(sustituido)\n");
        }

        fputc('\n', output);
    }

    fclose(output);

    printf("\nTest finalizado.\n");
    printf("Revisa el fichero 'test_output.txt'\n");

    return 0;
}
