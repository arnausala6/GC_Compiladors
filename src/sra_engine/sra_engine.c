#include "sra_engine.h"

void init_sra_engine(sra_engine *engine, TokenList *token_list, FILE *dbg_out){
    init_parser_automaton(&engine->automaton);
    init_stack(&engine->stack);
    engine->token_list = token_list;
    engine->dbg_out = dbg_out;
}

void run_sra_engine(sra_engine *engine){
    Token lookahead = get_next_token(&engine->token_list); //Lo implementa el módulo de token_list
    while(true){
        StackItem top = peek_stack(&engine->stack); //Lo implementa el módulo de stack
        char *lhs_symbol;
        int rhs_length;
        ActionType action = get_action(&engine->automaton, top.state, lookahead.type, &lhs_symbol, &rhs_length); //Lo implementa el módulo de automata
        if(action == ACT_SHIFT){
            int next_state = get_shift_state(&engine->automaton, top.state, lookahead.type); //Lo implementa el módulo de automata
            push_stack(&engine->stack, lookahead.type, next_state);
            lookahead = get_next_token(&engine->token_list);
        } else if(action == ACT_REDUCE){
            for(int i = 0; i < rhs_length; i++){
                pop_stack(&engine->stack); //Lo implementa el módulo de stack
            }
            StackItem prev_top = peek_stack(&engine->stack);
            int goto_state = get_goto_state(&engine->automaton, prev_top.state, lhs_symbol); //Lo implementa el módulo de automata
            push_stack(&engine->stack, lhs_symbol, goto_state);
        } else if(action == ACT_ACCEPT){
            fprintf(engine->dbg_out, "Análisis sintáctico completado exitosamente\n"); //cambiar esto para que se encargue módulo Debug
            return;
        } else if(action == ACT_ERROR){
            fprintf(engine->dbg_out, "Error: acción no válida para el símbolo en la cima de la pila\n"); //cambiar esto para que se encargue módulo Debug
            return;
        } else {
            fprintf(engine->dbg_out, "Error: acción desconocida\n"); //cambiar esto para que se encargue módulo Debug
            return;
        }
    }
}