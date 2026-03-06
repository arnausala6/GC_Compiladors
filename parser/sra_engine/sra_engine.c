#include "sra_engine.h"
#include "../language/language.h"

static ActionType sra_get_action_info(Language *lang, int state, int lookahead_symbol, int *lhs_symbol, int *rhs_length){
    ActionEntry entry = get_action(lang, state, lookahead_symbol);

    if (entry.type == ACT_REDUCE) {
        int production_id = entry.value;
        *lhs_symbol = lang->prods[production_id].lhs;
        *rhs_length = lang->prods[production_id].rhs_len;
    }

    return entry.type;
}

void init_sra_engine(SraEngine *engine, TokenStream *token_list, FILE *dbg_out){
    init_parser_automaton(&engine->automaton);
    stack_init(&engine->stack);
    engine->token_list = token_list;
    engine->dbg_out = dbg_out;
}

void run_sra_engine(Language *lang){
    const Token *tok_ptr = tokenstream_peek(lang->engine.token_list);
    Token lookahead;
    Dbg dbg;

    if (!tok_ptr) return;
    lookahead = *tok_ptr;

    dbg_init(&dbg, lang->engine.dbg_out);

    while (true) {
        StackItem top = stack_peek(&lang->engine.stack);
        int lhs_symbol = -1;
        int rhs_length = 0;

        ActionType action = sra_get_action_info(
            lang,
            top.state,
            lookahead.symbol,
            &lhs_symbol,
            &rhs_length
        );

        if (action == ACT_SHIFT) {
            ActionEntry entry = get_action(lang, top.state, lookahead.symbol);
            int next_state = entry.value;

            sra_do_shift(&lang->engine, &lookahead, next_state);

            dbg_print_step(
                &dbg,
                DBG_OP_SHIFT,
                lang->engine.token_list->pos,
                lookahead.lexeme,
                token_category_name(lookahead.category),
                top.state,
                next_state,
                &lang->engine.stack
            );
        }
        else if (action == ACT_REDUCE) {
            int s_new;
            sra_do_reduce(&lang->engine, lhs_symbol, rhs_length, &s_new);

            dbg_print_step(
                &dbg,
                DBG_OP_REDUCE,
                lang->engine.token_list->pos,
                lookahead.lexeme,
                token_category_name(lookahead.category),
                top.state,
                s_new,
                &lang->engine.stack
            );
        }
        else if (action == ACT_ACCEPT) {
            dbg_print_step(
                &dbg,
                DBG_OP_ACCEPT,
                lang->engine.token_list->pos,
                lookahead.lexeme,
                token_category_name(lookahead.category),
                top.state,
                DBG_STATE_NA,
                &lang->engine.stack
            );
            return;
        }
        else if (action == ACT_ERROR) {
            dbg_print_step(
                &dbg,
                DBG_OP_ERROR,
                lang->engine.token_list->pos,
                lookahead.lexeme,
                token_category_name(lookahead.category),
                top.state,
                DBG_STATE_NA,
                &lang->engine.stack
            );
            return;
        }
        else {
            return;
        }
    }
}

void sra_do_shift(SraEngine *engine, Token *lookahead, int next_state){
    const Token *next_tok;

    stack_push(&engine->stack, next_state, lookahead->symbol);

    tokenstream_advance(engine->token_list);
    next_tok = tokenstream_peek(engine->token_list);

    if (next_tok) {
        *lookahead = *next_tok;
    }
}

void sra_do_reduce(SraEngine *engine, int lhs_symbol, int rhs_length, int *s_new){
    int i;
    StackItem prev_top;

    for (i = 0; i < rhs_length; i++) {
        stack_pop(&engine->stack);
    }

    prev_top = stack_peek(&engine->stack);
    *s_new = get_goto(NULL, 0, 0);
    (void)prev_top;
    (void)lhs_symbol;
}