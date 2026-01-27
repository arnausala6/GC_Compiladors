/*
 archivo: t09_ifdef_deep_nested_include.c
 descripcion: test con ifdef anidados (3 niveles) e includes dentro y fuera de ifdef
*/

/* ================================
   DEFINES
   ================================ */

#define MODE_DEBUG 1
#define ENABLE_LOGS 1
/* #define EXPERIMENTAL_FEATURE 1 */   // <- prueba a comentar/descomentar

/* ================================
   CODIGO GLOBAL
   ================================ */

// Este comentario debe desaparecer si usas -c o -all
int valor = 10;

/* String con // dentro: NO debe interpretarse como comentario */
const char *url = "http://example.com/path"; // este comentario sí depende de flags

/* String con /* dentro: NO debe abrir comentario */
const char *fake = "text /* not a comment */ end";

/* ================================
   INCLUDE SIEMPRE (FUERA DE IFDEF)
   ================================ */

#include "../../tests/t4.h"

/* ================================
   IFDEF SIMPLE
   ================================ */

#ifdef ENABLE_LOGS
int logs_activados = 1;
#endif

/* ================================
   IFDEF CON INCLUDE DENTRO
   ================================ */

#ifdef MODE_DEBUG
int debug_flag = MODE_DEBUG;
#include "../../tests/t4.h"
#endif

/* ================================
   IFDEF ANIDADO (3 NIVELES)
   ================================ */

#ifdef MODE_DEBUG

void funcion_debug(void)
{
    valor = valor + 1;

    #ifdef ENABLE_LOGS
        valor = valor + 100;

        #ifdef EXPERIMENTAL_FEATURE
            // Solo aparece si EXPERIMENTAL_FEATURE está definido
            valor = valor + 1000;
            #include "../../tests/t4.h"
        #endif

        valor = valor + 200;
    #endif

    valor = valor + 2;
}

#endif

/* ================================
   CODIGO FINAL
   ================================ */

/* Comentario bloque: debe desaparecer si -c o -all */
int fin = 1;

/* Fin del archivo */
