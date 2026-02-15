#include "token_model.h"
#include <string.h>

void tokenlist_init(TokenList *tl) {
    if (!tl) return;
    tl->size = 0;
}

void tokenlist_clear(TokenList *tl) {
    if (!tl) return;
    tl->size = 0;
}

static int clamp_len(int len) {
    if (len < 0) return 0;
    if (len >= TOKEN_LEXEME_MAX) return TOKEN_LEXEME_MAX - 1;
    return len;
}

int tokenlist_add(
    TokenList *tl,
    SrcLoc loc,
    const char *lexeme,
    int len,
    TokenCategory category,
    const Counters *counters
) {
    if (!tl || tl->size >= TOKENLIST_CAPACITY) {
        return 0;
    }

    Token *t = &tl->data[tl->size];

    t->loc = loc;
    t->category = category;

    int n = clamp_len(len);
    if (lexeme && n > 0) {
        memcpy(t->lexeme, lexeme, (size_t)n);
    }
    t->lexeme[n] = '\0';

    if (counters) {
        t->counters_at_emit = *counters;
    } else {
        counters_init(&t->counters_at_emit);
    }

    t->index = tl->size;
    tl->size += 1;
    return 1;
}

/* Valores alineados con enum TokenCategory en automata_engine.h (CAT_NUMBER=0, ...). */
const char *token_category_name(TokenCategory cat) {
    switch (cat) {
        case 0: return "CAT_NUMBER";
        case 1: return "CAT_IDENTIFIER";
        case 2: return "CAT_KEYWORD";
        case 3: return "CAT_LITERAL";
        case 4: return "CAT_OPERATOR";
        case 5: return "CAT_SPECIALCHAR";
        case 6: return "CAT_NONRECOGNIZED";
        default: return "CAT_UNKNOWN";
    }
}
