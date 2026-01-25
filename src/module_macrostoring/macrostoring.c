#include "macrostoring.h"

void guardar_macro(Tabla_macros *tabla_macros, char *nombre, char *cuerpo){
    Macro **tmp = realloc(tabla_macros->macros, (tabla_macros->elementos + 1) * sizeof(Macro *));
    if (!tmp) {
        perror("realloc failed");
        exit(1);
    }
    tabla_macros->macros = tmp;
    
    Macro *m = malloc(sizeof(Macro));
    if (!m) { 
        perror("malloc failed"); 
        exit(1); 
    }

    tabla_macros->macros[tabla_macros->elementos] = m;

    tabla_macros->macros[tabla_macros->elementos]->nombre = strdup(nombre);
    
    tabla_macros->macros[tabla_macros->elementos]->cuerpo = strdup(cuerpo);

    tabla_macros->elementos++;
}
