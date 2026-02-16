#ifndef SCANNER_CORE_H
#define SCANNER_CORE_H

#include <stdio.h>
#include "../automata_engine/automata_engine.h"
#define LEX_MAX 256
/* Forward declarations (evita includes circulares) */
struct AutomataSet;
struct TokenList;
struct Diagnostics;
struct Counters;

typedef struct {
    const char *file;
    int line;
    int column;
} SrcLoc;

typedef struct {
    FILE *input;
    SrcLoc loc;

    char lexeme_buffer[LEX_MAX];
    int  lexeme_length;

    DFA *automata;  // Puntero a conjunto de autómatas (DFA) para el scanner
    struct TokenList *tokens;
    struct Diagnostics *diag;
    struct Counters *counters;

    int pending_has;     
    int pending_char;    
    SrcLoc pending_loc;  

} Scanner;


void scanner_init(
    Scanner *s,
    FILE *input,
    const char *filename,
    struct TokenList *tokens,
    struct Diagnostics *diag,
    struct Counters *counters
);

int scanner_run(Scanner *s);

#endif
