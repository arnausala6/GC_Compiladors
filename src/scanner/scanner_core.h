#ifndef SCANNER_CORE_H
#define SCANNER_CORE_H

#include <stdio.h>

/* Forward declarations (evita includes circulares) */
struct AutomataSet;
struct TokenList;
struct Diagnostics;
struct Counters;

/* --- Según slides --- */
typedef struct {
    const char *file;
    int line;
    int column;
} SrcLoc;

typedef struct {
    FILE *input;
    SrcLoc loc;

    /* Buffer del lexema en construcción */
    char lexeme_buffer[256];
    int  lexeme_length;

    /* Dependencias */
    struct AutomataSet *automata;
    struct TokenList *tokens;
    struct Diagnostics *diag;
    struct Counters *counters;

    /* Lookahead sin consumir */
    int pending_has;     /* 0/1 */
    int pending_char;    /* valor del char pendiente */
    SrcLoc pending_loc;  /* localización del char pendiente */

} Scanner;

/* Init + run */
void scanner_init(
    Scanner *s,
    FILE *input,
    const char *filename,
    struct AutomataSet *automata,
    struct TokenList *tokens,
    struct Diagnostics *diag,
    struct Counters *counters
);

/* Ejecuta el escaneo completo, llenando TokenList. */
int scanner_run(Scanner *s);

#endif /* SCANNER_CORE_H */
