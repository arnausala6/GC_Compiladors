#include "sra_engine.h"

void init_sra_engine(sra_engine *engine, TokenList *token_list, FILE *dbg_out){
    init_parser_automaton(&engine->automaton);
    stack_init(&engine->stack);
    engine->token_list = token_list;
    engine->dbg_out = dbg_out;
}

void run_sra_engine(Language *lang){
    Token lookahead = get_next_token(lang->engine->token_list); //Lo implementa el módulo de token_list
    while(true){
        StackItem top = stack_peek(lang->engine->stack); //Lo implementa el módulo de stack
        char *lhs_symbol;
        int rhs_length;
        ActionType action = get_action(lang, top.state, lookahead.type, &lhs_symbol, &rhs_length); 
        if(action == ACT_SHIFT){
            int next_state = get_shift_state(&lang->engine->automaton, top.state, lookahead.type); //Lo implementa el módulo de automata
            sra_do_shift(lang->engine, &lookahead, next_state);
            dbg_print_step(dbg, DBG_OP_SHIFT, token_list->pos, lookahead.lexeme, token_category_name(lookahead.category), top.state, next_state, lang->engine->stack);
        } else if(action == ACT_REDUCE){
            int s_new;
            sra_do_reduce(lang->engine, lhs_symbol, rhs_length, &s_new);
            dbg_print_step(dbg, DBG_OP_REDUCE, token_list->pos, lookahead.lexeme, token_category_name(lookahead.category), top.state, s_new, lang->engine->stack);
        } else if(action == ACT_ACCEPT){
            dbg_print_step(dbg, DBG_OP_ACCEPT, token_list->pos, lookahead.lexeme, token_category_name(lookahead.category), top.state, DBG_STATE_NA, lang->engine->stack);
            return;
        } else if(action == ACT_ERROR){
            dbg_print_step(dbg, DBG_OP_ERROR, token_list->pos, lookahead.lexeme, token_category_name(lookahead.category), top.state, DBG_STATE_NA, lang->engine->stack); 
            return;
        } else {
            return;
        }
    }
}

void sra_do_shift(sra_engine *engine, Token *lookahead, int next_state){
    stack_push(&engine->stack, lookahead->type, next_state);
    *lookahead = get_next_token(&engine->token_list);
}

void sra_do_reduce(sra_engine *engine, char *lhs_symbol, int rhs_length, int *s_new){
    for(int i = 0; i < rhs_length; i++){
        stack_pop(&engine->stack); //Lo implementa el módulo de stack
    }
    StackItem prev_top = stack_peek(&engine->stack);
    *s_new = get_goto(&engine->automaton, prev_top.state, lhs_symbol);
    stack_push(&engine->stack, lhs_symbol, *s_new);
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