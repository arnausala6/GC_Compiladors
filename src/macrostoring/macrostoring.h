#ifndef MACROSTORING_H
#define MACROSTORING_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    char *nombre;
    char *cuerpo;
} Macro;

typedef struct {
    int elementos;
    Macro **macros;
}Tabla_macros;

void guardar_macro(Tabla_macros *tabla_macros, char *nombre, char *cuerpo);

#endif // MACROSTORING_H