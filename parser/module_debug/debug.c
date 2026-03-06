/*
 * Programa: P3 – Bottom-up Parsing (Shift-Reduce Automaton)
 * Archivo:  debug.c
 *
 * Descripción:
 *   Implementación del módulo de traza de depuración.  Formatea y escribe
 *   una línea por paso del motor SRA.  El módulo es "write-only": no modifica
 *   la pila, el flujo de tokens ni ningún estado del parser.
 */

#include "debug.h"

/* ------------------------------------------------------------------ */
/*  Helpers estáticos                                                  */
/* ------------------------------------------------------------------ */

static void dbg_write_la(FILE *out,
                         const char *lexeme,
                         const char *category)
{
    char buffer[50]; // Buffer temporal para formatear el token

    if (!lexeme && !category) {
        snprintf(buffer, sizeof(buffer), "<->");
    } else {
        snprintf(buffer, sizeof(buffer), "<%s, %s>",
                lexeme   ? lexeme   : "-",
                category ? category : "-");
    }

    // Imprimimos el buffer alineado a la izquierda ocupando 20 espacios
    fprintf(out, "%-20s", buffer);
}

static void dbg_write_stack(FILE *out, const Stack *st)
{
    fprintf(out, "[");
    if (st) {
        for (int i = 0; i < st->top; i++) {
            if (i > 0)
                fprintf(out, " ");
            fprintf(out, "%d:%d", st->data[i].state, st->data[i].symbol);
        }
    }
    fprintf(out, "]");
}

/* ------------------------------------------------------------------ */
/*  API pública                                                        */
/* ------------------------------------------------------------------ */

int dbg_init(Dbg *dbg, FILE *out)
{
    if (!dbg) return 1;
    dbg->out  = out;
    dbg->step = 0;
    if (dbg->out)
        // Ajustamos los espacios para que coincidan con los anchos definidos abajo
        fprintf(dbg->out,
                "STEP | OP      | POS | LA                   | S_prev | S_new | STACK\n");
    return 0;
}

void dbg_close(Dbg *dbg)
{
    if (!dbg || !dbg->out) return;
    fflush(dbg->out);
}

void dbg_print_step(Dbg *dbg,
                    const char *op,
                    int pos,
                    const char *la_lexeme,
                    const char *la_category,
                    int s_prev,
                    int s_new,
                    const Stack *st)
{
    if (!dbg || !dbg->out) return;

    // STEP (4 espacios) | OP (7 espacios alineado a la izq) | POS (3 espacios) |
    fprintf(dbg->out, "%-4d | %-7s | %-3d | ",
            dbg->step, op ? op : "-", pos);

    // LA (Lookahead)
    dbg_write_la(dbg->out, la_lexeme, la_category);

    // S_prev (6 espacios) | S_new (5 espacios) |
    fprintf(dbg->out, " | %-6d | %-5d | ", s_prev, s_new);

    // STACK
    dbg_write_stack(dbg->out, st);

    fprintf(dbg->out, "\n");

    dbg->step++;
}
