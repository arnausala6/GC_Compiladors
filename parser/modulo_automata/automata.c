#include "automata.h"

ActionEntry get_action(Language *lang, int state, int terminal_symbol)
{
    return lang->dfa_tables.action[state][terminal_symbol];
}

int get_goto(Language *lang, int state, int nonterminal_symbol)
{
    return lang->dfa_tables.goto_table[state][nonterminal_symbol];
}
