#include <utils_files.h>
#include <stdbool.h>

typedef struct{
    ParserAutomaton automaton;
    Stack stack;
    TokenList *token_list;
    FILE *dbg_out;
}sra_engine;