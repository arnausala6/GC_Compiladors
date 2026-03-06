#include <stdio.h>
#include "tokenlist.h"

static void print_token(const Token *tok, int i) {
    if (!tok) return;

    printf("[%d] lexeme='%s' category=%s symbol=%d\n",
           i,
           tok->lexeme,
           token_category_name(tok->category),
           tok->symbol);
}

int main(void) {
    TokenStream ts;
    const Token *tok;

    if (tokenlist_load(&ts, "test.cscn") != 0) {
        printf("ERROR: tokenlist_load failed\n");
        return 1;
    }

    printf("LOAD OK\n");
    printf("size = %d\n", ts.size);
    printf("pos = %d\n", ts.pos);

    for (int i = 0; i < ts.size; i++) {
        print_token(&ts.tokens[i], i);
    }

    tok = tokenstream_peek(&ts);
    if (tok) {
        printf("peek(0) = %s\n", tok->lexeme);
    } else {
        printf("peek(0) = NULL\n");
    }

    tokenstream_advance(&ts);
    tok = tokenstream_peek(&ts);
    if (tok) {
        printf("after advance, peek = %s\n", tok->lexeme);
    } else {
        printf("after advance, peek = NULL\n");
    }

    tokenstream_reset(&ts);
    tok = tokenstream_peek(&ts);
    if (tok) {
        printf("after reset, peek = %s\n", tok->lexeme);
    } else {
        printf("after reset, peek = NULL\n");
    }

    tokenstream_free(&ts);
    printf("FREE OK\n");

    return 0;
}