#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <stdio.h>
#include "scanner_core.h"   /* SrcLoc (única definición, evita duplicado con scanner) */

#ifdef __cplusplus
extern "C" {
#endif

/* --- Según diapositivas: CompilerPhase, ErrorId, Diagnostic --- */

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

/** Añade un error (guarda Diagnostic y escribe en d->out). loc según diapos. */
int diagnostics_add_error(
    Diagnostics *d,
    ErrorId id,
    CompilerPhase phase,
    const SrcLoc *loc,
    const char *details
);

/** Igual pasando file/line/column (para el scanner que ya tiene SrcLoc). */
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

#ifdef __cplusplus
}
#endif

#endif /* DIAGNOSTICS_H */
