#include "output_writer.h"
#include <stdio.h>

static void sync_lines(OutputWriter *ow, int token_line) {
    if (!ow || !ow->output) return;
    if (token_line < 1) token_line = 1;

    while (ow->last_line_written < token_line) {
        fputc('\n', ow->output);
        ow->last_line_written++;
    }
}

void output_writer_init(OutputWriter *ow, FILE *out, OutFormat format, int debug_on) {
    if (!ow) return;
    ow->output = out;
    ow->format = format;
    ow->debug_on = debug_on;
    ow->last_line_written = 1;
}

void output_writer_write_token(OutputWriter *ow, const Token *tok) {
    if (!ow || !ow->output || !tok) return;

    /* Mantener correspondencia de líneas con el input */
    sync_lines(ow, tok->loc.line);

    if (ow->format == OUT_DEBUG) {
        /* DEBUG: una línea por token, con localización y lexema */
        if (ow->debug_on) {
            /* debug extra (file:line:col) */
            const char *fname = tok->loc.file ? tok->loc.file : "<nofile>";
            fprintf(ow->output, "%s:%d:%d  %s  \"%s\"\n",
                    fname,
                    tok->loc.line,
                    tok->loc.column,
                    token_category_name(tok->category),
                    tok->lexeme);
        } else {
            /* debug “light” */
            fprintf(ow->output, "[%d:%d] %s \"%s\"\n",
                    tok->loc.line,
                    tok->loc.column,
                    token_category_name(tok->category),
                    tok->lexeme);
        }
    } else {
        /* RELEASE: compacto (categorías separadas por espacio) */
        fprintf(ow->output, "<%s, %s> ", tok->lexeme, token_category_name(tok->category));
    }

    ow->last_line_written = tok->loc.line;
}

void output_writer_close(OutputWriter *ow) {
    if (!ow || !ow->output) return;

    if (ow->format == OUT_RELEASE) {
        fputc('\n', ow->output);
    }

    fflush(ow->output);
}
