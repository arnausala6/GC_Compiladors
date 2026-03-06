/*
Programa: P3 – Bottom-up Parsing
Archivo: tokenlist.h
Autor: Arnau Sala
Fecha: 4/3/2025
Descripción: Modelo de tokens del parser.
             Este módulo carga el fichero .cscn generado por el scanner y construye un
             TokenStream secuencial consumido por el SRA engine mediante lookahead.
*/

#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <stdio.h>

#define TOKEN_LEXEME_MAX 256
#define TOKENSTREAM_INITIAL_CAPACITY 32

typedef enum {
    CAT_NUMBER = 0,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERATOR,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED,
    CAT_EOF,
    CAT_UNKNOWN
} TokenCategory;

typedef struct {
    char lexeme[TOKEN_LEXEME_MAX];
    TokenCategory category;
    int symbol;
} Token;

typedef struct {
    Token *tokens;
    int size;
    int pos;
    int capacity;
} TokenStream;

int tokenlist_load(TokenStream *ts, const char *path); //carga el .cscn
void tokenstream_free(TokenStream *ts);
void tokenstream_reset(TokenStream *ts);
const Token *tokenstream_peek(const TokenStream *ts);
void tokenstream_advance(TokenStream *ts);
int tokenstream_is_end(const TokenStream *ts);
const char *token_category_name(TokenCategory cat);

#endif /* TOKENLIST_H */