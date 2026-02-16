/*
  Nombre del programa: Módulo de Diagnostics (Interfaz)
 
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 10 de febrero de 2026
 
  Descripción:
  Define los tipos y la interfaz del módulo de diagnóstico de errores.
  Los errores tienen un identificador único (ErrorId) y un step (CompilerPhase).
  El mismo mensaje se reutiliza para el mismo error; details permite añadir
  parámetros (localización, datos concretos). La salida se redirige según la
  directiva de preprocesador DEBUG: OFF (0) → stdout; ON (1) → fichero de salida.

  Responsabilidades:
  - Definir CompilerPhase, ErrorId, Diagnostic y Diagnostics.
  - Declarar inicialización (con elección de salida según DEBUG) y registro de errores.

  Notas de implementación:
  - Siempre se usa fprintf (nunca printf). El manejador de salida se fija en init.
  - Quién llama a diagnostics_init debe pasar (stdout, fichero_salida); el módulo
    elige uno u otro según DEBUG.
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

void diagnostics_init(Diagnostics *d, FILE *when_debug_off, FILE *when_debug_on);

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

#endif
