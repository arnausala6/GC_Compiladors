#ifndef SRA_ENGINE_H
#define SRA_ENGINE_H

#include <stdbool.h>
#include "../module_stack/stack.h"
#include "../module_debug/debug.h"
#include "../modulo_automata/automata.h"
#include "../modulo_tokenlist/tokenlist.h"

typedef struct{
    ParserAutomaton automaton;
    Stack stack;
    TokenStream *token_list;
    FILE *dbg_out;
} SraEngine;

void init_sra_engine(SraEngine *engine, TokenStream *token_list, FILE *dbg_out);
void run_sra_engine(Language *lang);
void sra_do_shift(SraEngine *engine, Token *lookahead, int next_state);
void sra_do_reduce(SraEngine *engine, int lhs_symbol, int rhs_length, int *s_new);

#endif /* SRA_ENGINE_H */
