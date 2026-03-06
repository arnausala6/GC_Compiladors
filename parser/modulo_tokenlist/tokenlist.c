#include "tokenlist.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void tokenstream_init(TokenStream *ts)
{
    if (!ts) return;

    ts->tokens = NULL;
    ts->size = 0;
    ts->pos = 0;
    ts->capacity = 0;
}

static int tokenstream_reserve(TokenStream *ts, int new_capacity)
{
    Token *new_tokens;

    if (!ts) return 1;
    if (new_capacity <= ts->capacity) return 0;

    new_tokens = (Token *)realloc(ts->tokens, (size_t)new_capacity * sizeof(Token));
    if (!new_tokens) return 1;

    ts->tokens = new_tokens;
    ts->capacity = new_capacity;
    return 0;
}

static int tokenstream_push(TokenStream *ts, const Token *tok)
{
    int new_capacity;

    if (!ts || !tok) return 1;

    if (ts->size >= ts->capacity) {
        new_capacity = (ts->capacity == 0) ? TOKENSTREAM_INITIAL_CAPACITY : ts->capacity * 2;

        if (tokenstream_reserve(ts, new_capacity) != 0) {
            return 1;
        }
    }

    ts->tokens[ts->size] = *tok;
    ts->size++;
    return 0;
}

static void token_copy_lexeme(Token *tok, const char *src)
{
    if (!tok) return;

    if (!src) {
        tok->lexeme[0] = '\0';
        return;
    }

    strncpy(tok->lexeme, src, TOKEN_LEXEME_MAX - 1);
    tok->lexeme[TOKEN_LEXEME_MAX - 1] = '\0';
}

static int is_blank_line(const char *line)
{
    if (!line) return 1;

    while (*line) {
        if (!isspace((unsigned char)*line)) return 0;
        line++;
    }

    return 1;
}

static TokenCategory parse_category_name(const char *cat_str)
{
    if (!cat_str) return CAT_UNKNOWN;

    if (strcmp(cat_str, "CAT_NUMBER") == 0) return CAT_NUMBER;
    if (strcmp(cat_str, "CAT_IDENTIFIER") == 0) return CAT_IDENTIFIER;
    if (strcmp(cat_str, "CAT_KEYWORD") == 0) return CAT_KEYWORD;
    if (strcmp(cat_str, "CAT_LITERAL") == 0) return CAT_LITERAL;
    if (strcmp(cat_str, "CAT_OPERATOR") == 0) return CAT_OPERATOR;
    if (strcmp(cat_str, "CAT_SPECIALCHAR") == 0) return CAT_SPECIALCHAR;
    if (strcmp(cat_str, "CAT_NONRECOGNIZED") == 0) return CAT_NONRECOGNIZED;
    if (strcmp(cat_str, "CAT_EOF") == 0) return CAT_EOF;

    return CAT_UNKNOWN;
}

static int token_symbol_from_category_and_lexeme(TokenCategory cat, const char *lexeme)
{
    if (!lexeme) return -1;

    switch (cat) {
        case CAT_NUMBER:
            return 4; /* NUM */

        case CAT_OPERATOR:
            if (strcmp(lexeme, "+") == 0) return 0;
            if (strcmp(lexeme, "*") == 0) return 1;
            break;

        case CAT_SPECIALCHAR:
            if (strcmp(lexeme, "(") == 0) return 2;
            if (strcmp(lexeme, ")") == 0) return 3;
            if (strcmp(lexeme, "$") == 0) return 5;
            break;

        case CAT_EOF:
            return 5;

        default:
            break;
    }

    return -1;
}

static int parse_token_line(const char *line, Token *tok)
{
    char category_buf[64];
    char lexeme_buf[TOKEN_LEXEME_MAX];
    char *p;
    int n;
    TokenCategory cat;

    if (!line || !tok) return 1;

    category_buf[0] = '\0';
    lexeme_buf[0] = '\0';

    n = sscanf(line, " %63s %255[^\n]", category_buf, lexeme_buf);
    if (n <= 0) return 1;

    cat = parse_category_name(category_buf);
    if (cat == CAT_UNKNOWN) return 1;

    if (n == 1) {
        if (cat == CAT_EOF) {
            strcpy(lexeme_buf, "$");
        } else {
            lexeme_buf[0] = '\0';
        }
    } else {
        p = lexeme_buf;
        while (*p && isspace((unsigned char)*p)) p++;

        if (p != lexeme_buf) {
            memmove(lexeme_buf, p, strlen(p) + 1);
        }
    }

    tok->category = cat;
    token_copy_lexeme(tok, lexeme_buf);
    tok->symbol = token_symbol_from_category_and_lexeme(cat, tok->lexeme);

    return 0;
}

int tokenlist_load(TokenStream *ts, const char *path)
{
    FILE *f;
    char line[512];

    if (!ts || !path) return 1;

    tokenstream_init(ts);

    f = fopen(path, "r");
    if (!f) return 1;

    while (fgets(line, sizeof(line), f)) {
        Token tok;

        if (is_blank_line(line)) continue;

        if (parse_token_line(line, &tok) != 0) {
            fclose(f);
            tokenstream_free(ts);
            return 1;
        }

        if (tok.symbol < 0) {
            fclose(f);
            tokenstream_free(ts);
            return 1;
        }

        if (tokenstream_push(ts, &tok) != 0) {
            fclose(f);
            tokenstream_free(ts);
            return 1;
        }
    }

    fclose(f);

    if (ts->size == 0 || ts->tokens[ts->size - 1].symbol != 5) {
        Token eof_tok;

        eof_tok.category = CAT_EOF;
        strcpy(eof_tok.lexeme, "$");
        eof_tok.symbol = 5;

        if (tokenstream_push(ts, &eof_tok) != 0) {
            tokenstream_free(ts);
            return 1;
        }
    }

    ts->pos = 0;
    return 0;
}

void tokenstream_free(TokenStream *ts)
{
    if (!ts) return;

    free(ts->tokens);
    ts->tokens = NULL;
    ts->size = 0;
    ts->pos = 0;
    ts->capacity = 0;
}

void tokenstream_reset(TokenStream *ts)
{
    if (!ts) return;
    ts->pos = 0;
}

const Token *tokenstream_peek(const TokenStream *ts)
{
    if (!ts) return NULL;
    if (ts->pos < 0 || ts->pos >= ts->size) return NULL;

    return &ts->tokens[ts->pos];
}

void tokenstream_advance(TokenStream *ts)
{
    if (!ts) return;
    if (ts->pos < ts->size) ts->pos++;
}

int tokenstream_is_end(const TokenStream *ts)
{
    if (!ts) return 1;
    return ts->pos >= ts->size;
}

const char *token_category_name(TokenCategory cat)
{
    switch (cat) {
        case CAT_NUMBER: return "CAT_NUMBER";
        case CAT_IDENTIFIER: return "CAT_IDENTIFIER";
        case CAT_KEYWORD: return "CAT_KEYWORD";
        case CAT_LITERAL: return "CAT_LITERAL";
        case CAT_OPERATOR: return "CAT_OPERATOR";
        case CAT_SPECIALCHAR: return "CAT_SPECIALCHAR";
        case CAT_NONRECOGNIZED: return "CAT_NONRECOGNIZED";
        case CAT_EOF: return "CAT_EOF";
        default: return "CAT_UNKNOWN";
    }
}