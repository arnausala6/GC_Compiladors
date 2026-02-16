/*
  Nombre del programa: Módulo de Diagnostics (Interfaz)
 
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 10 de febrero de 2026
 
  Descripción:
  Este archivo define los tipos y la interfaz del módulo de diagnóstico de errores
  del compilador. Permite registrar errores con su identificador, fase del
  compilador, localización en el código y un mensaje opcional.
 
  Responsabilidades:
  - Definir los enums de fase y de error (CompilerPhase, ErrorId).
  - Definir las estructuras Diagnostic y Diagnostics.
  - Declarar funciones para inicializar y registrar diagnósticos.
 
  Notas de implementación:
  - No usa memoria dinámica: los diagnósticos se guardan en un array fijo.
  - El stream de salida se configura al inicializar (stdout o fichero).
  - details puede ser NULL para usar el mensaje por defecto del error.
 */

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <stdio.h>
#include "../scanner/scanner_core.h"

typedef enum CompilerPhase {
    PHASE_SCANNER
} CompilerPhase;

typedef enum ErrorId {
    ERR_NONRECOGNIZED,
    ERR_UNCLOSED_LITERAL,
    ERR_INVALID_IDENTIFIER,
    ERR_LEXEME_TOO_LONG
} ErrorId;

typedef struct {
    ErrorId id;
    CompilerPhase phase;
    SrcLoc loc;
    const char *details;
} Diagnostic;

#define DIAGLIST_CAPACITY 4096

typedef struct Diagnostics {
    FILE *out;
    Diagnostic data[DIAGLIST_CAPACITY];
    int size;
} Diagnostics;

void diagnostics_init(Diagnostics *d, FILE *out);

int diagnostics_add_error(
    Diagnostics *d,
    ErrorId id,
    CompilerPhase phase,
    const SrcLoc *loc,
    const char *details
);

int diagnostics_add_error_at(
    Diagnostics *d,
    ErrorId id,
    CompilerPhase phase,
    const char *file,
    int line,
    int column,
    const char *details
);

int diagnostics_count(const Diagnostics *d);
const Diagnostic *diagnostics_get(const Diagnostics *d, int i);
const char *diagnostics_default_message(ErrorId id);

/**
 * Imprime un resumen de diagnósticos.
 * Muestra el total de diagnósticos y su clasificación.
 */
void diagnostics_print_summary(const Diagnostics *d, FILE *out);

#endif
