/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: guardar_directivas.h
* Autor: Wassim Ibn Tahar
* Fecha: 18/01/2026
*
* Descripción:
* Este archivo define la interfaz del módulo de guardar y parseo de directivas
* Proporciona las declaraciones necesarias para analizar
* líneas que contienen directivas del preprocesador y representarlas de forma
* estructurada mediante el tipo Directiva.
*
* -----------------------------------------------------------------------------
*/

#ifndef GUARDAR_DIRECTIVAS_H
#define GUARDAR_DIRECTIVAS_H

#include "directivas_types.h"
#include "../module_preprocessor/pp_shared.h"


#ifdef __cplusplus
extern "C" {
#endif

//util para otros módulos
int guardar_directiva_parse_line(const char *line, SrcLoc loc, Directiva *out, GDError *err);
                                 
#ifdef __cplusplus
}
#endif

#endif
