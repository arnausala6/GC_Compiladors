#include "sra_engine.h"

void init_sra_engine(sra_engine *engine, TokenList *token_list, FILE *dbg_out){
    init_parser_automaton(&engine->automaton);
    init_stack(&engine->stack);
    engine->token_list = token_list;
    engine->dbg_out = dbg_out;
}

void run_sra_engine(Language *lang){
    Token lookahead = get_next_token(lang->engine->token_list); //Lo implementa el módulo de token_list
    while(true){
        StackItem top = peek_stack(lang->engine->stack); //Lo implementa el módulo de stack
        char *lhs_symbol;
        int rhs_length;
        ActionType action = get_action(lang, top.state, lookahead.type, &lhs_symbol, &rhs_length); //Lo implementa el módulo de automata
        if(action == ACT_SHIFT){
            int next_state = get_shift_state(&lang->engine->automaton, top.state, lookahead.type); //Lo implementa el módulo de automata
            sra_do_shift(lang->engine, &lookahead, next_state);
            sra_debug_print(lang->engine, /*argumentos por definir*/); //lo implementa el módulo de Debug
        } else if(action == ACT_REDUCE){
            sra_do_reduce(lang->engine, lhs_symbol, rhs_length);
            sra_debug_print(lang->engine, /*argumentos por definir*/); //lo implementa el módulo de Debug
        } else if(action == ACT_ACCEPT){
            sra_debug_print(/*argumentos por definir*/); //lo implementa el módulo de Debug
            return;
        } else if(action == ACT_ERROR){
            sra_debug_print(/*argumentos por definir*/); //lo implementa el módulo de Debug
            return;
        } else {
            sra_debug_print(/*argumentos por definir*/); //lo implementa el módulo de Debug
            return;
        }
    }
}

void sra_do_shift(sra_engine *engine, Token *lookahead, int next_state){
    push_stack(&engine->stack, lookahead->type, next_state);
    *lookahead = get_next_token(&engine->token_list);
}

void sra_do_reduce(sra_engine *engine, char *lhs_symbol, int rhs_length){
    for(int i = 0; i < rhs_length; i++){
        pop_stack(&engine->stack); //Lo implementa el módulo de stack
    }
    StackItem prev_top = peek_stack(&engine->stack);
    int goto_state = get_goto_state(&engine->automaton, prev_top.state, lhs_symbol); //Lo implementa el módulo de automata
    push_stack(&engine->stack, lhs_symbol, goto_state);
}

ActionType get_action(Language *lang, int state, TokenCategory lookahead, char **lhs_symbol, int *rhs_length){
    ActionType action = lang->automaton->action[state][lookahead].type;
    if(action == ACT_SHIFT){
        lang->automaton->current_state = lang->automaton->action[state][lookahead].value;
    }

    else if(action == ACT_REDUCE){
        int production_id = lang->dfa_tables.action[state][lookahead].value;
        int nonterminal_id = lang->prods[production_id].lhs;
        *lhs_symbol = lang->nonterminals[nonterminal_id]; 
        *rhs_length = lang->prods[production_id].rhs_length;
    }

    return action;
}