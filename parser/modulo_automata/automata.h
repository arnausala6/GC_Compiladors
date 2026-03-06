#ifndef AUTOMATA_H
#define AUTOMATA_H

/* --- Tipos de acción del autómata SRA --- */

typedef enum {
    ACT_ERROR = 0,
    ACT_SHIFT,
    ACT_REDUCE,
    ACT_ACCEPT
} ActionType;

typedef struct {
    ActionType type;
    int value;
} ActionEntry;

typedef struct {
    int current_state;
} ParserAutomaton;

typedef struct Language Language;

void init_parser_automaton(ParserAutomaton *pa);

/* ACTION[state][terminal_symbol] */
ActionEntry get_action(Language *lang, int state, int terminal_symbol);

/* GOTO[state][nonterminal_symbol] */
int get_goto(Language *lang, int state, int nonterminal_symbol);

#endif
