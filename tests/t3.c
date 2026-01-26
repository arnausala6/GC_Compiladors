/*
 archivo: t08_ifdef_nested_include.c
 descripcion: test con ifdef anidados e includes
*/

/* ================================
   DEFINES
   ================================ */

#define MODE_DEBUG 1

/* ================================
   CODIGO GLOBAL
   ================================ */

int valor = 1;

/* ================================
   IFDEF SIMPLE
   ================================ */

#ifdef ENABLE_LOGS
int logs_activados = 1;
#endif

/* ================================
   IFDEF CON INCLUDE
   ================================ */
int b =33;
#include "..\..\tests\t4.h"
int a = 22;
/* ================================
   IFDEF ANIDADO
   ================================ */

#ifdef MODE_DEBUG
void funcion_ejemplo(void)
{
    valor = valor + 1;

    #ifdef ENABLE_LOGS
        valor = valor + 100;
    #endif

    valor = valor + 2;
}
#endif

/* Fin del archivo */
