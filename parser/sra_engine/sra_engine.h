#include <utils_files.h>
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