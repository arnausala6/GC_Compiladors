#include <utils_files.h>
#include <stdbool.h>
#include "../module_stack/stack.h"
#include "../module_debug/debug.h"
#include "../modulo_automata/automata.h"

typedef struct{
    ParserAutomaton automaton;
    Stack stack;
    TokenList *token_list;
    FILE *dbg_out;
}sra_engine;