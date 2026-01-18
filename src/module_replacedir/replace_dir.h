#pragma once
#include <stdio.h>
#include "directivas_types.h"
#include "guardar_directivas.h"   // debe declarar guardar_directivas_parse_line(...)
#include "macrostoring.h"         // Tabla_macros, guardar_macro(...)

/* Flags (ajústalos a vuestro proyecto si ya los tenéis) */
enum {
  PP_FLAG_COMMENTS    = 1 << 0,  // -c
  PP_FLAG_DIRECTIVES  = 1 << 1,  // -d
  PP_FLAG_ALL         = 1 << 2   // -all (o mapearlo a COMMENTS|DIRECTIVES)
};

/* Error simple para este módulo */
typedef struct {
  SrcLoc loc;
  char msg[256];
} DSError;

/* Stack para #ifdef/#endif (simple y suficiente para P1PP) */
#define DS_MAX_IF_DEPTH 64
typedef struct {
  int active[DS_MAX_IF_DEPTH];  // 1=activo, 0=inactivo
  int depth;                    // 0..DS_MAX_IF_DEPTH-1
} IfStack;

/* Contexto compartido durante el preprocesado */
typedef struct {
  const char   *current_file;   // nombre del archivo actual (para loc y debug)
  Tabla_macros *macros;         // tabla de macros global (se comparte con includes)
  IfStack       ifs;            // estado de ifdefs
} PPContext;

/* Inicialización */
void ppcontext_init(PPContext *ctx, const char *current_file, Tabla_macros *macros);

/* Estado del if-stack */
int  ifs_is_active(const IfStack *st);
int  ifs_push(IfStack *st, int cond_true);
int  ifs_pop(IfStack *st);

/* Callback para procesar includes recursivos:
   Debe abrir el archivo 'path' y llamar al motor principal sobre ese FILE*,
   escribiendo al mismo out y compartiendo ctx (macros + stack).
*/
// typedef int (*PPProcessFileFn)(const char *path, FILE *out, int flags, PPContext *ctx, DSError *err);

/* Función principal del módulo 4:
   Llamar cuando el motor ha visto un '#' (primer no blanco de línea).
   Esta función consume el resto de la línea de la directiva desde 'in'
   y aplica su efecto.
*/
int replace_directives_handle_hash(
  FILE *in, FILE *out,
  int *linea_actual,
  int flags,
  PPContext *ctx,
  PPProcessFileFn process_file_fn,
  DSError *err
);
