#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include <stdio.h>
#include "token_model.h"

typedef enum {
    OUT_RELEASE = 0,
    OUT_DEBUG   = 1
} OutFormat;

typedef struct {
    FILE *output;            /* stream de salida (lo abre/cierra el driver) */
    OutFormat format;        /* release o debug */
    int debug_on;            
    int last_line_written;   /* para sincronizar líneas del input -> output */
} OutputWriter;

void output_writer_init(OutputWriter *ow, FILE *out, OutFormat format, int debug_on);

void output_writer_write_token(OutputWriter *ow, const Token *tok);

void output_writer_close(OutputWriter *ow);

#endif
