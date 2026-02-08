/*
 * Archivo: macrostoring.c
 * Programa: P1PP – Preprocesador
 * Autor: Ivan Jimenez
 * Fecha: 17/01/2026
 * Descripción:
 * Implementación del módulo de almacenamiento de macros del preprocesador.
 * Este archivo contiene las funciones encargadas de crear y gestionar la
 * tabla de macros, añadiendo nuevas definiciones obtenidas a partir de
 * directivas #define.
 *
 * El módulo se limita a reservar memoria y guardar el nombre y el cuerpo
 * textual de cada macro. No realiza expansión ni sustitución de macros,
 * ni interactúa directamente con la lectura del archivo de entrada.
 */


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