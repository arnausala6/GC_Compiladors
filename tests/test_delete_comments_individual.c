/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: test_delete_comments_individual.c
* Autor: Arnau Sala
* Fecha: 21/01/2026
*
* Descripción:
* Test individual del módulo delete_comments.
* Permite comprobar de forma aislada que la eliminación de comentarios
* funciona correctamente sin depender del resto del preprocesador.
*
* El test lee un archivo de entrada, elimina los comentarios y genera
* un archivo de salida para su verificación manual.
* -----------------------------------------------------------------------------
*/


/*
EL TEST FUNCIONA, PERO SOLO SIRVE PARA EL MODULO DE ELIMINAR COMENTARIOS DE FORMA AISLADA
Para ejecutar, abrimos una terminal desde la raiz del proyecto (.../GC_Compiladors) y hacemos lo siguiente:

1. Compilamos con el siguiente comando
gcc -Wall -Wextra -std=c11 src\delete_comments\delete_comments.c tests\test_delete_comments_individual.c -o test_delete_comments_individual.exe

2. Ejecutamos el archivo .exe creado
.\test_delete_comments_individual.exe

el archivo output_test_individual estará en la carpeta tests del modulo delete_comments
*/


#include <stdio.h>
#include "../src/delete_comments/delete_comments.h"

int main(void) {
    FILE *in = fopen("src/delete_comments/tests/input_comments.c", "r");
    FILE *out = fopen("src/delete_comments/tests/output_test_individual.c", "w");
    int linea = 1;
    int rc;

    if (!in) {
        perror("src/delete_comments/tests/input_comments.c");
        return 1;
    }
    if (!out) {
        perror("src/delete_comments/tests/output_test_individual.c");
        fclose(in);
        return 1;
    }

    rc = procesar_fichero_sin_comentarios(in, out, &linea);

    fclose(in);
    fclose(out);

    if (rc != 0) {
        fprintf(stderr, "ERROR: procesar_fichero_sin_comentarios devolvió %d\n", rc);
        return 1;
    }

    printf("OK. Lineas procesadas: %d\n", linea);
    printf("Revisa output_test_individual.c\n");
    return 0;
}