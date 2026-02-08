/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: test_delete_comments.c
* Autor: Arnau Sala
* Fecha: 21/01/2026
*
* Descripción:
* Test de integración del módulo delete_comments dentro del motor del
* preprocesador.
* Permite comprobar que la eliminación de comentarios funciona cuando
* el módulo es llamado desde el preprocesador completo.
*
* Nota:
* Otros elementos del preprocesador (macros, directivas) pueden afectar
* al resultado, pero este test se centra en validar la eliminación de
* comentarios.
* -----------------------------------------------------------------------------
*/


/*
EL TEST FUNCIONA, PERO BORRA COSAS DE MAS --> ESO ES UN TEMA DE MACROS Y DIRECTIVAS. LOS COMENTARIOS FUNCIONAN BIEN 
Para ejecutar, abrimos una terminal desde la raiz del proyecto (.../GC_Compiladors) y hacemos lo siguiente:

1. Compilamos con el siguiente comando
gcc -Wall -Wextra -std=c11 src\module_preprocessor\preprocessor.c src\delete_comments\delete_comments.c tests\test_delete_comments.c -o test_delete_coments.exe

2. Ejecutamos el archivo .exe creado
.\test_delete_comments.exe

el archivo _pp estará en la carpeta tests del modulo delete_comments
*/

#include <stdio.h>
#include "../preprocessor/module_preprocessor/preprocessor.h"

int main(void) {
    FILE *in = fopen("src/delete_comments/tests/input_comments.c", "r");
    if (!in) { perror("input_comments.c"); return 1; }

    FILE *out = fopen("src/delete_comments/tests/output_comments_pp.c", "w");
    if (!out) { perror("output_comments_pp.c"); fclose(in); return 1; }

    int flags = 0; // de momento se queda asi

    motor_preprocesador(in, out, flags, "src/delete_comments/tests/input_comments.c", NULL, NULL, NULL);

    fclose(in);
    fclose(out);

    printf("OK. Revisa output_comments_pp.c\n");
    return 0;
}