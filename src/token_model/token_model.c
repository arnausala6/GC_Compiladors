/*
* -----------------------------------------------------------------------------
* Programa: P2 – Lexer
* Archivo: token_model.c
* Autor: Arnau Sala
* Fecha: 9/02/2026
*
 * Descripción:
 *  Este archivo implementa el modelo de tokens, que es responsable de mantener
 *  la estructura de datos para almacenar los tokens reconocidos por el scanner.
 * 
 * El modelo de tokens incluye:
 * - La definición de la estructura TokenList, que es una lista de tokens con capacidad fija.
 * - Funciones para inicializar, limpiar y agregar tokens a la TokenList.
* -----------------------------------------------------------------------------
*/
#include "token_model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef MAX_LENGTH
#define MAX_LENGTH 64
#endif

static void rstrip(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || isspace((unsigned char)s[n - 1]))) {
        s[n - 1] = '\0';
        n--;
    }
}

/* .cscn esperado (según vuestra slide de ejemplo):
 *   NUMBER 5
 *   OP +
 *   NUMBER 3
 *   EOF $
 *
 * Si vuestro scanner usa otros tags, ajusta aquí.
 */
static TokenCategory parse_category(const char *type_str) {
    if (!type_str) return CAT_NONRECOGNIZED;

    if (strcmp(type_str, "NUMBER") == 0) return CAT_NUMBER;
    if (strcmp(type_str, "OP") == 0) return CAT_OPERATOR;
    if (strcmp(type_str, "EOF") == 0) return CAT_SPECIALCHAR;

    /* Otros posibles (por si vuestro .cscn los usa) */
    if (strcmp(type_str, "IDENT") == 0   || strcmp(type_str, "ID")  == 0) return CAT_IDENTIFIER;
    if (strcmp(type_str, "KEYWORD") == 0 || strcmp(type_str, "KW")  == 0) return CAT_KEYWORD;
    if (strcmp(type_str, "LITERAL") == 0 || strcmp(type_str, "LIT") == 0) return CAT_LITERAL;

    return CAT_NONRECOGNIZED;
}

/* Mapeo a terminal ID del parser (columna ACTION).
 * Orden por defecto (fallback) basado en ejemplo típico:
 *   %TERMINALS: + * ( ) NUM $
 * => IDs:        0 1 2 3  4  5
 * TODO(P3): confirmar IDs de terminales según language/automata.
 */
static int map_to_terminal_id(TokenCategory cat, const char *lexeme) {
    if (cat == CAT_NUMBER) {
        return 4; /* NUM_ID */
    }

    if (!lexeme) return -1;

    if (strcmp(lexeme, "+") == 0) return 0;
    if (strcmp(lexeme, "*") == 0) return 1;
    if (strcmp(lexeme, "(") == 0) return 2;
    if (strcmp(lexeme, ")") == 0) return 3;
    if (strcmp(lexeme, "$") == 0) return 5;

    return -1;
}

static bool ensure_capacity(TokenList *tl, int *cap) {
    if (tl->size < *cap) return true;
    int new_cap = (*cap == 0) ? 16 : (*cap * 2);
    Token *new_arr = (Token *)realloc(tl->tokens, (size_t)new_cap * sizeof(Token));
    if (!new_arr) return false;
    tl->tokens = new_arr;
    *cap = new_cap;
    return true;
}

void tokenlist_init(TokenList *tl) {
    if (!tl) return;
    tl->tokens = NULL;
    tl->size = 0;
    tl->pos = 0;
}

void tokenlist_free(TokenList *tl) {
    if (!tl) return;
    free(tl->tokens);
    tl->tokens = NULL;
    tl->size = 0;
    tl->pos = 0;
}

void tokenlist_reset(TokenList *tl) {
    if (!tl) return;
    tl->pos = 0;
}

bool tokenlist_load(TokenList *tl, const char *path) {
    if (!tl || !path) return false;

    /* Reiniciar por si se reutiliza */
    tokenlist_free(tl);
    tokenlist_init(tl);

    FILE *f = fopen(path, "r");
    if (!f) return false;

    int cap = 0;
    char line[256];
    int line_no = 0;

    while (fgets(line, sizeof(line), f)) {
        line_no++;
        rstrip(line);
        if (line[0] == '\0') continue;

        /* TYPE LEXEME (ejemplo: NUMBER 5, OP +, EOF $)
         * Si la línea tiene formato incorrecto, se ignora sin abortar.
         */
        char type_str[MAX_LENGTH] = {0};
        char lex[MAX_LENGTH] = {0};
        int parsed = sscanf(line, " %63s %63s", type_str, lex);
        if (parsed <= 0) {
            continue;
        }
        if (parsed == 1) {
            continue;
        }

        Token t;
        memset(&t, 0, sizeof(t));

        t.category = parse_category(type_str);
        strncpy(t.lexeme, lex, MAX_LENGTH - 1);
        t.lexeme[MAX_LENGTH - 1] = '\0';
        t.loc.file = path;
        t.loc.line = line_no;
        t.loc.column = 1;

        t.type = map_to_terminal_id(t.category, t.lexeme);

        /* Si no podemos mapear a un terminal del parser, marcamos como no reconocido */
        if (t.type < 0) {
            t.category = CAT_NONRECOGNIZED;
            t.type = -1;
        }

        if (!ensure_capacity(tl, &cap)) {
            fclose(f);
            tokenlist_free(tl);
            return false;
        }

        tl->tokens[tl->size++] = t;
    }

    fclose(f);

    /* TODO(P3): confirmar si EOF siempre viene en .cscn o se añade en tokenlist_load.
     * Por ahora NO se añade automáticamente para respetar el flujo externo.
     */

    return true;
}

const Token *tokenlist_peek(const TokenList *tl) {
    if (!tl || !tl->tokens) return NULL;
    if (tl->pos < 0 || tl->pos >= tl->size) return NULL;
    return &tl->tokens[tl->pos];
}

/* Firma compatible con vuestro engine: get_next_token(&engine->token_list)
 * donde engine->token_list es TokenList*
 */
Token get_next_token(TokenList **ptl) {
    Token empty;
    memset(&empty, 0, sizeof(empty));
    empty.category = CAT_NONRECOGNIZED;
    empty.type = -1;
    empty.lexeme[0] = '\0';
    empty.loc.file = NULL;
    empty.loc.line = 0;
    empty.loc.column = 0;

    if (!ptl || !*ptl) return empty;
    TokenList *tl = *ptl;

    if (!tl->tokens || tl->pos < 0 || tl->pos >= tl->size) {
        return empty;
    }

    Token t = tl->tokens[tl->pos];
    tl->pos += 1; /* consumir */
    return t;
}

const char *token_category_name(TokenCategory cat) {
    switch (cat) {
        case CAT_NUMBER: return "CAT_NUMBER";
        case CAT_IDENTIFIER: return "CAT_IDENTIFIER";
        case CAT_KEYWORD: return "CAT_KEYWORD";
        case CAT_LITERAL: return "CAT_LITERAL";
        case CAT_OPERATOR: return "CAT_OPERATOR";
        case CAT_SPECIALCHAR: return "CAT_SPECIALCHAR";
        case CAT_NONRECOGNIZED: return "CAT_NONRECOGNIZED";
        default: return "CAT_UNKNOWN";
    }
}

// TODO(P3): CHECKLIST INTEGRACION
// - Verificar IDs reales de terminales con language.txt/automata y ajustar map_to_terminal_id.
// - Confirmar formato real del .cscn (nombres de tipo y separador exacto TYPE LEXEME).
// - Confirmar estrategia de EOF: si debe venir siempre en .cscn o añadirse externamente.