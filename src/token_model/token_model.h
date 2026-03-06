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

#include <stdbool.h>

#if defined(__has_include)
#if __has_include("../automata_engine/automata_engine.h")
#include "../automata_engine/automata_engine.h"
#endif
#endif

/* Ajusta si en vuestro proyecto ya existe MAX_LENGTH */
#ifndef MAX_LENGTH
#define MAX_LENGTH 64
#endif

/* Categorías: si automata_engine ya las define, reutilizarlas para evitar conflictos. */
#ifndef TOKEN_CATEGORY_FROM_AUTOMATA_ENGINE
typedef enum {
    CAT_NUMBER = 0,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERATOR,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED
} TokenCategory;
#define TOKEN_CATEGORY_FROM_AUTOMATA_ENGINE 1
#endif

/* Compatibilidad con módulos legacy que aún leen localización del token. */
typedef struct {
    const char *file;
    int line;
    int column;
} TokenLoc;

/* Token para P3:
 * - lexeme: útil para debug/trace
 * - category: viene del scanner (P2)
 * - type: id del terminal del parser (columna ACTION). En slides se llama "symbol".
 */
typedef struct {
    char lexeme[MAX_LENGTH];
    TokenCategory category;
    int type; /* id del terminal (parser) */
    TokenLoc loc;
} Token;

/* TokenList = TokenStream en vuestras slides:
 * - tokens: array dinámico
 * - size: número de tokens cargados
 * - pos: índice del lookahead
 */
typedef struct {
    Token *tokens;
    int size;
    int pos;
} TokenList;

/* Construcción / destrucción */
void tokenlist_init(TokenList *tl);
void tokenlist_free(TokenList *tl);
void tokenlist_reset(TokenList *tl);

/* Cargar tokens desde .cscn (salida del scanner) */
bool tokenlist_load(TokenList *tl, const char *path);

/* Lookahead (no consume) */
const Token *tokenlist_peek(const TokenList *tl);

/* Consumir: firma compatible con vuestro sra_engine (TokenList **). */
Token get_next_token(TokenList **tl);

/* Utilidad opcional */
const char *token_category_name(TokenCategory cat);

#endif /* TOKEN_MODEL_H */