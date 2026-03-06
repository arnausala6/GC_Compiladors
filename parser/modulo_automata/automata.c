#include "automata.h"
#include "../language/language.h"

void init_parser_automaton(ParserAutomaton *pa)
{
    if (!pa) return;
    pa->current_state = 0;
}

ActionEntry get_action(Language *lang, int state, int terminal_symbol)
{
    return lang->dfa_tables.action[state][terminal_symbol];
}

int get_goto(Language *lang, int state, int nonterminal_symbol)
{
    return lang->dfa_tables.goto_table[state][nonterminal_symbol];
}
