/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: replace_dir.h
* Autor: Yeray Espinosa
* Fecha: 17/01/2026
*
* Descripción:
 *  Este archivo declara la interfaz del módulo encargado de la gestión y
 *  ejecución de directivas del preprocesador.
 *
 *  Proporciona funciones para:
 *   - Controlar el estado de bloques condicionales (#ifdef / #endif)
 *     mediante una pila de activación.
 *   - Ejecutar directivas del preprocesador una vez detectadas y parseadas,
 *     interactuando con la tabla de macros y el sistema de errores.
 *
 *  Este módulo no realiza lectura directa del archivo de entrada; es invocado
 *  exclusivamente por el motor del preprocesador.
 * 
* -----------------------------------------------------------------------------
*/


#pragma once
#include <stdio.h>
#include "../module_preprocessor/pp_shared.h" 
#include "../guardar_directivas/guardar_directivas.h"   // debe declarar guardar_directivas_parse_line(...)
#include "../macrostoring/macrostoring.h"         // Tabla_macros, guardar_macro(...)

#define MAX_DIRECTIVE_LINE_LENGTH 2048
#define MAX_INCLUDE_PATH_LENGTH 1024


/* Estado del if-stack */
int ifs_init(IfStack *st);
int  ifs_is_active(const IfStack *st);
int  ifs_push(IfStack *st, int cond_true);
int  ifs_pop(IfStack *st);


int replace_directives_handle_hash(
  FILE *in, FILE *out, int *lineactual, int flags, Tabla_macros *macros, GDError *err, IfStack *ifstack, const char *current_file);
