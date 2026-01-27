/*
 * Archivo: pp_shared.h
 * Programa: P1PP – Preprocesador
 * Autor: [Varios autores de módulos]
 * Fecha: 17/01/2026
 * Descripción:
 * Definiciones compartidas entre los distintos módulos del preprocesador.
 * Este archivo centraliza las estructuras comunes usadas para la gestión
 * de errores, localización (archivo, línea, columna) y el estado de
 * condicionales de preprocesado (#ifdef / #endif) mediante un stack.
 *
 * Su objetivo es evitar dependencias circulares y duplicación de tipos
 * entre módulos.
 */


#ifndef PP_SHARED_H
#define PP_SHARED_H

#ifdef __cplusplus
extern "C" {
#endif

// --- Localización de errores / debug ---
typedef struct {
  const char *file;
  int line;
  int col;
} SrcLoc;

// --- Error genérico usado entre módulos ---
typedef struct {
  SrcLoc loc;
  char msg[256];
} GDError;

// --- Stack de condicionales (#ifdef/#endif) ---
#define DS_MAX_IF_DEPTH 64
typedef struct {
  int active[DS_MAX_IF_DEPTH];
  int depth;
} IfStack;

#ifdef __cplusplus
}
#endif

#endif // PP_SHARED_H
