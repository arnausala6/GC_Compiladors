#pragma once
#include <stdio.h>
#include "directivas_types.h"
#include "guardar_directivas.h"   // debe declarar guardar_directivas_parse_line(...)
#include "macrostoring.h"         // Tabla_macros, guardar_macro(...)


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
  const Directiva *d, FILE *out, Tabla_macros *macros, GDError *err, IfStack *ifstack
);
