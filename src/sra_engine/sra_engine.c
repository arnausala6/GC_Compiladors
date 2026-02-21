#include "sra_engine.h"

void init_sra_engine(sra_engine *engine, FILE *dbg_out){
    init_parser_automaton(&engine->automaton);
    init_stack(&engine->stack);
    init_token_list(&engine->token_list);
    engine->dbg_out = dbg_out;
}

void run_sra_engine(sra_engine *engine){
    Token current_token = get_next_token(&engine->token_list); //Lo implementa el módulo de token_list
    while(current_token.lexeme != '$'){
        char *top = peek_stack(&engine->stack); //Lo implementa el módulo de stack
        char *lhs_symbol;
        int rhs_length;
        SymbolType top_type = get_symbol_type(top); //Lo implementa el módulo de Language
        ActionType action = get_action(&engine->automaton, top, top_type, &lhs_symbol, &rhs_length); //Lo implementa el módulo de automata
        if(action == ACT_SHIFT){
            push_stack(&engine->stack, current_token.lexeme); //Lo implementa el módulo de stack
            current_token = get_next_token(&engine->token_list);
        } else if(action == ACT_REDUCE){
            for(int i = 0; i < rhs_length; i++){
                pop_stack(&engine->stack); //Lo implementa el módulo de stack
            }
            push_stack(&engine->stack, lhs_symbol);
        } else if(action == ACT_ACCEPT){
            fprintf(engine->dbg_out, "Análisis sintáctico completado exitosamente\n");
            return;
        } else if(action == ACT_ERROR){
            fprintf(engine->dbg_out, "Error: acción no válida para el símbolo en la cima de la pila\n");
            return;
        } else {
            fprintf(engine->dbg_out, "Error: acción desconocida\n");
            return;
        }
    }
}