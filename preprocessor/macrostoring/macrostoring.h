/*
 * Archivo: macrostoring.h
 * Programa: P1PP – Preprocesador
 * Autor: Ivan Jimenez
 * Fecha: 17/01/2026
 * Descripción:
 * Definiciones del módulo de almacenamiento de macros del preprocesador.
 * Este módulo se encarga de mantener una tabla global de macros definidas
 * mediante directivas #define, almacenando su nombre y su cuerpo textual.
 *
 * Proporciona las estructuras necesarias para representar macros y la
 * tabla de macros, así como la interfaz para registrar nuevas definiciones.
 * No realiza sustitución de macros ni lectura de archivos.
 */


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