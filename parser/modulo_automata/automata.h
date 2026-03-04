#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "language.h"   // tiene Language, ActionEntry, MAX_* y dfa_tables

typedef struct {
    int current_state;
} ParserAutomaton;

/* ACTION[state][terminal_symbol] */
ActionEntry get_action(Language *lang, int state, int terminal_symbol);

/* GOTO[state][nonterminal_symbol] */
int get_goto(Language *lang, int state, int nonterminal_symbol);

#endif
