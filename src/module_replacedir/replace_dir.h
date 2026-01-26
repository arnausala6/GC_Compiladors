#pragma once
#include <stdio.h>
#include "directivas_types.h"
#include "guardar_directivas.h"   // debe declarar guardar_directivas_parse_line(...)
#include "macrostoring.h"         // Tabla_macros, guardar_macro(...)

#define MAX_DIRECTIVE_LINE_LENGTH 2048
#define MAX_INCLUDE_PATH_LENGTH 1024

/* Stack para #ifdef/#endif (simple y suficiente para P1PP) */
#define DS_MAX_IF_DEPTH 64
typedef struct {
  int active[DS_MAX_IF_DEPTH];  // 1=activo, 0=inactivo
  int depth;                    // 0..DS_MAX_IF_DEPTH-1
} IfStack;

/* Estado del if-stack */
int  ifs_is_active(const IfStack *st);
int  ifs_push(IfStack *st, int cond_true);
int  ifs_pop(IfStack *st);


int replace_directives_handle_hash(
  FILE *in, FILE *out, int *lineactual, int flags, Tabla_macros *macros, GDError *err, IfStack *ifstack, const char *current_file,
);
