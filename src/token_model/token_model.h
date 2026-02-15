#ifndef TOKEN_MODEL_H
#define TOKEN_MODEL_H

#include <stddef.h>
#include "scanner_core.h" 
#include "counters.h"       

//Tipo de categoría: usar enum de automata_engine.h si está disponible; si no, fallback int.
#ifndef TOKEN_CATEGORY_FROM_AUTOMATA_ENGINE
typedef int TokenCategory;
#endif

#define TOKENLIST_CAPACITY 50000
#define TOKEN_LEXEME_MAX 256

typedef struct {
    char lexeme[TOKEN_LEXEME_MAX];
    TokenCategory category;
    SrcLoc loc;
    Counters counters_at_emit;
    int index;
} Token;

typedef struct {
    Token data[TOKENLIST_CAPACITY];
    int size;
} TokenList;

void tokenlist_init(TokenList *tl);
void tokenlist_clear(TokenList *tl);

int tokenlist_add(
    TokenList *tl,
    SrcLoc loc,
    const char *lexeme,
    int len,
    TokenCategory category,
    const Counters *counters
);

static inline int tokenlist_size(const TokenList *tl) {
    return tl ? tl->size : 0;
}

static inline const Token *tokenlist_get(const TokenList *tl, int i) {
    if (!tl || i < 0 || i >= tl->size) return NULL;
    return &tl->data[i];
}

const char *token_category_name(TokenCategory cat);

#endif // TOKEN_MODEL_H
