/*
  Nombre del programa: Módulo de Diagnostics (Implementación)

  Autor(es): Iván Torres Ros

  Fecha de creación: 10 de febrero de 2026

  Descripción:
  Implementa el registro de errores y su escritura en el stream elegido según
  la directiva DEBUG (OFF → stdout, ON → fichero de salida). Usa solo fprintf.

  Responsabilidades:
  - Inicializar Diagnostics y fijar d->out según DEBUG.
  - Añadir errores (guardar Diagnostic y escribir en d->out).
  - Mensajes por defecto reutilizables por ErrorId; details como parámetros opcionales.

  Notas de implementación:
  - Si DEBUG no está definido se asume 0 (salida a stdout).
  - Si details es NULL o vacío se usa el mensaje por defecto del ErrorId.
 */

#include "diagnostics.h"
#include <stdio.h>

#ifndef DEBUG
#define DEBUG 0
#endif

static const char *const default_messages[] = {
    [ERR_NONRECOGNIZED]      = "non-recognized lexeme",
    [ERR_UNCLOSED_LITERAL]   = "unclosed literal",
    [ERR_INVALID_IDENTIFIER] = "invalid identifier",
    [ERR_LEXEME_TOO_LONG]    = "lexeme buffer overflow"
};

void diagnostics_init(Diagnostics *d, FILE *when_debug_off, FILE *when_debug_on) {
    if (!d) return;
    d->size = 0;
#if (DEBUG == 1)
    d->out = when_debug_on ? when_debug_on : stdout;
#else
    d->out = when_debug_off ? when_debug_off : stdout;
#endif
}

static const char *message_for(ErrorId id, const char *details) {
    if (details && details[0] != '\0') return details;
    return default_messages[id];
}

static void write_diagnostic(FILE *out, const Diagnostic *diag) {
    const char *msg = message_for(diag->id, diag->details);
    const char *f = diag->loc.file ? diag->loc.file : "(unknown)";
    fprintf(out, "%s:%d:%d: %s\n", f, diag->loc.line, diag->loc.column, msg);
}

int diagnostics_add_error(
    Diagnostics *d,
    ErrorId id,
    CompilerPhase phase,
    const SrcLoc *loc,
    const char *details
) {
    if (!d || !loc || d->size >= DIAGLIST_CAPACITY) return 0;
    Diagnostic *t = &d->data[d->size];
    t->id = id;
    t->phase = phase;
    t->loc = *loc;
    t->details = details;
    if (d->out) write_diagnostic(d->out, t);
    d->size += 1;
    return 1;
}

int diagnostics_add_error_at(
    Diagnostics *d,
    ErrorId id,
    CompilerPhase phase,
    const char *file,
    int line,
    int column,
    const char *details
) {
    SrcLoc loc = { .file = file, .line = line, .column = column };
    return diagnostics_add_error(d, id, phase, &loc, details);
}

int diagnostics_count(const Diagnostics *d) {
    return d ? d->size : 0;
}

const Diagnostic *diagnostics_get(const Diagnostics *d, int i) {
    if (!d || i < 0 || i >= d->size) return NULL;
    return &d->data[i];
}

const char *diagnostics_default_message(ErrorId id) {
    if ((unsigned)id >= sizeof(default_messages) / sizeof(default_messages[0]))
        return "unknown error";
    return default_messages[id];
}
