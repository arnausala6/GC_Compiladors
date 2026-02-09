#include "scanner_core.h"

#include <string.h>
#include <stdlib.h>
#define LEXEME_MAX 256

static void advance_loc(SrcLoc *loc, int ch) {
    if (ch == '\n') {
        loc->line += 1;
        loc->column = 1;
    } else {
        loc->column += 1;
    }
}

static int is_delimiter(int ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' ||
            ch == '\r' || ch == '\v' || ch == '\f');
}

static void lexeme_clear(Scanner *s) {
    s->lexeme_length = 0;
    s->lexeme_buffer[0] = '\0';
}

static int lexeme_push(Scanner *s, int ch) {
    if (s->lexeme_length >= LEXEME_MAX - 1) {
        /* Aquí se debería registrar error de lexema demasiado largo */
        /* Se encarga el módulo de errores
        diagnostics_add_error(
            s->diag,
            ERR_LEXEME_TOO_LONG,
            PHASE_SCANNER,
            s->loc,
            "lexeme buffer overflow"
        );
        */
        return 0;
    }

    s->lexeme_buffer[s->lexeme_length] = (char)ch;
    s->lexeme_length++;
    s->lexeme_buffer[s->lexeme_length] = '\0';
    return 1;
}

// Lee un char, actualizando la localización. Si hay un char pendiente, lo devuelve sin leer de input.

static int read_char(Scanner *s, SrcLoc *char_loc) {
    if (s->pending_has) {
        s->pending_has = 0;
        if (char_loc) *char_loc = s->pending_loc;
        return s->pending_char;
    }

    int ch = fgetc(s->input);
    if (ch == EOF) return EOF;

    if (char_loc) *char_loc = s->loc;
    advance_loc(&s->loc, ch);
    return ch;
}

static void set_pending(Scanner *s, int ch, SrcLoc loc) {
    s->pending_has = 1;
    s->pending_char = ch;
    s->pending_loc = loc;
}

 
static void emit_token(
    Scanner *s,
    SrcLoc start_loc,
    int token_category,   /* enum TokenCategory */
    int length            /* longitud válida del lexema */
) {
    /* Aquí el Token Model debería crear y almacenar el token */
    /*
    tokenlist_add(     
        s->tokens,
        start_loc,
        s->lexeme_buffer,
        length,
        token_category
    );
    */

}

static int scan_next_token(Scanner *s) {
    lexeme_clear(s);

    /* 1) Saltar delimitadores fuera de tokens */
    while (1) {
        SrcLoc loc0;
        int ch = read_char(s, &loc0);
        if (ch == EOF) return 0;

        if (is_delimiter(ch)) {
            continue;
        }

        set_pending(s, ch, loc0);
        break;
    }

    /* 2) Inicializar DFA */

    /*
    DfaState state = automata_reset(s->automata); módulo de autómatas se encarga de esto
    */

    SrcLoc token_start;
    int started = 0;

    int last_accept_len = -1;
    int last_accept_state = -1;

    /* 3) Leer caracteres guiados por el DFA */
    while (1) {
        SrcLoc ch_loc;
        int ch = read_char(s, &ch_loc);

        if (ch == EOF) {
            if (last_accept_len >= 0) {
                /*
                TokenCategory cat = automata_category_for(s->automata, last_accept_state); se encarga el módulo de autómatas
                */
                emit_token(s, token_start, /*cat*/ 0, last_accept_len);
                return 1;
            }
            return 0;
        }

        if (!started) {
            token_start = ch_loc;
            started = 1;
        }

        /* Paso de DFA */
        /*
        DfaState next = automata_step(s->automata, state, ch); módulo de autómatas se encarga de esto
        */

        /* if (next == DFA_FAIL) */
        if (0) { /* ← condición real la pone el módulo de autómatas */
            set_pending(s, ch, ch_loc);

            if (last_accept_len >= 0) {
                /*
                TokenCategory cat = automata_category_for(s->automata, last_accept_state); se encarga el módulo de autómatas
                */
                emit_token(s, token_start, /*cat*/ 0, last_accept_len);
                return 1;
            }

            /* No hubo ningún estado final: token no reconocido */
            lexeme_clear(s);
            lexeme_push(s, ch);

            /*
            diagnostics_add_error(
                s->diag,
                ERR_NONRECOGNIZED,
                PHASE_SCANNER,
                ch_loc,
                "non-recognized lexeme"
            );
            */

            emit_token(s, ch_loc, /*CAT_NONRECOGNIZED*/ 0, s->lexeme_length);
            return 1;
        }

        /* state = next; */
        lexeme_push(s, ch);

        /* Si el DFA está en estado final, lo recordamos */
        /*
        if (automata_is_accepting(s->automata, state)) {
            last_accept_state = state;
            last_accept_len = s->lexeme_length;
        }
        */
    }
}

void scanner_init(
    Scanner *s,
    FILE *input,
    const char *filename,
    struct AutomataSet *automata,
    struct TokenList *tokens,
    struct Diagnostics *diag,
    struct Counters *counters
) {
    memset(s, 0, sizeof(*s));

    s->input = input;
    s->loc.file = filename;
    s->loc.line = 1;
    s->loc.column = 1;

    s->automata = automata;
    s->tokens = tokens;
    s->diag = diag;
    s->counters = counters;

    s->pending_has = 0;
    lexeme_clear(s);
}

int scanner_run(Scanner *s) {
    while (1) {
        int r = scan_next_token(s);
        if (r == 0) break;
    }
    return 0;
}
