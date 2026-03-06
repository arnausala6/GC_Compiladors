/*
 * -----------------------------------------------------------------------------
 * Programa: P2 – Lexer
 * Archivo: token_model.h
 * Autor: Arnau Sala
 * Fecha: 9/02/2026
 *
 * Descripción:
 *  Este archivo define la interfaz pública del modelo de tokens. Proporciona
 *  las estructuras de datos y funciones necesarias para almacenar los tokens
 *  reconocidos por el scanner en una lista con capacidad fija.
 *
 *  El modelo de tokens incluye:
 *  - Definición de TOKENLIST_CAPACITY y TOKEN_LEXEME_MAX.
 *  - Estructuras Token y TokenList (lista de tokens con capacidad fija).
 *  - Funciones para inicializar/limpiar la lista y añadir tokens.
 *  - Funciones inline de acceso: tamaño de la lista y obtención por índice.
 *  - Conversión de categoría a string (token_category_name).
 * -----------------------------------------------------------------------------
 */
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
