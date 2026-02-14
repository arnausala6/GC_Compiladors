#include "scanner_core.h"

#include <string.h>
#include <stdlib.h>
#define LEXEME_MAX 256
#include "../automat_engine/automata_engine.h"

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

    /* 2) Inicializar el motor de autómatas (todos los DFAs en paralelo) */
    automata_engine_reset();

    SrcLoc token_start;
    int started = 0;

    int last_accept_len = -1;
    TokenCategory last_accept_category = CAT_NONRECOGNIZED;

    /* 3) Leer caracteres y preguntar al motor si quedan DFAs vivos */
    while (1) {
        SrcLoc ch_loc;
        int ch = read_char(s, &ch_loc);

        if (ch == EOF) {
            if (last_accept_len >= 0) {
                emit_token(s, token_start, (int)last_accept_category, last_accept_len);
                return 1;
            }
            return 0;
        }

        if (!started) {
            token_start = ch_loc;
            started = 1;
        }

        int any_alive = 0;
        int any_accepting = 0;
        TokenCategory best_accepting = CAT_NONRECOGNIZED;

        automata_engine_step(ch, &any_alive, &any_accepting, &best_accepting);

        if (any_alive) {
            // Al menos un DFA sigue vivo, agregar char al lexema y continuar.
            lexeme_push(s, ch);

            if (any_accepting) {
                last_accept_len = s->lexeme_length;
                last_accept_category = best_accepting;
            }
        } else {
            // No quedan DFAs vivos después de leer este char.
            if (last_accept_len >= 0) {
                set_pending(s, ch, ch_loc);
                emit_token(s, token_start, (int)last_accept_category, last_accept_len);
                return 1;
            } else {
                // No se reconoció ningún token, pero tampoco quedan DFAs vivos. Emitir error de caracter no reconocido.
                lexeme_clear(s);
                lexeme_push(s, ch);
                emit_token(s, ch_loc, (int)CAT_NONRECOGNIZED, s->lexeme_length);
                return 1;
            }
        }
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
