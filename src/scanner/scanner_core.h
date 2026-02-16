#ifndef SCANNER_CORE_H
#define SCANNER_CORE_H

#include <stdio.h>
#define LEX_MAX 256

struct DFA;
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

    struct DFA *automata;  /* puntero a conjunto de DFAs; definición en automata_engine.h */
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
