#include "replace_dir.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static void ds_set_error(DSError *err, SrcLoc loc, const char *msg) {
  if (!err) return;
  err->loc = loc;
  snprintf(err->msg, sizeof(err->msg), "%s", (msg ? msg : "Unknown error"));
}

/*IfStack*/

void ppcontext_init(PPContext *ctx, const char *current_file, Tabla_macros *macros) {
  if (!ctx) return;
  ctx->current_file = current_file ? current_file : "<no-file>";
  ctx->macros = macros;

  ctx->ifs.depth = 0;
  ctx->ifs.active[0] = 1; // nivel base activo
}

int ifs_is_active(const IfStack *st) {
  if (!st) return 1; // si no hay stack, asumimos activo
  return st->active[st->depth] ? 1 : 0;
}

int ifs_push(IfStack *st, int cond_true) {
  if (!st) return 1;
  if (st->depth >= DS_MAX_IF_DEPTH - 1) return 1;

  int parent_active = st->active[st->depth];
  st->depth++;
  st->active[st->depth] = (parent_active && cond_true) ? 1 : 0;
  return 0;
}

int ifs_pop(IfStack *st) {
  if (!st) return 1;
  if (st->depth <= 0) return 1; // endif sin ifdef
  st->depth--;
  return 0;
}



/* Leer resto de línea tras '#' */

static int read_line_after_hash(FILE *in, char *buf, size_t cap, int *linea_actual) {
  if (!in || !buf || cap == 0) return 1;

  if (!fgets(buf, (int)cap, in)) {
    // EOF justo después de '#'
    buf[0] = '\0';
    return 0;
  }

  // si había '\n', hemos consumido fin de línea
  if (strchr(buf, '\n')) {
    if (linea_actual) (*linea_actual)++;
  }

  return 0;
}

/* Función principal */

int replace_directives_handle_hash(
  FILE *in, FILE *out,
  int *linea_actual,
  int flags,
  PPContext *ctx,
  PPProcessFileFn process_file_fn,
  DSError *err
) {
  if (!in || !out || !ctx || !linea_actual) {
    SrcLoc loc = { ctx ? ctx->current_file : "<no-file>", 0, 0 };
    ds_set_error(err, loc, "bad args");
    return 1;
  }

  /* Guardar línea original (para copiar en directivas desconocidas) */
  char line_raw[4096];
  if (read_line_after_hash(in, line_raw, sizeof(line_raw), linea_actual) != 0) {
    SrcLoc loc = { ctx->current_file, *linea_actual, 1 };
    ds_set_error(err, loc, "cannot read directive line");
    return 1;
  }

  /* Si NO estamos procesando directivas (ni -d ni -all),
     entonces la línea se copia tal cual como texto normal, si el bloque está activo.
  */
  int do_directives = ((flags & PP_FLAG_DIRECTIVES) || (flags & PP_FLAG_ALL));
  if (!do_directives) {
    if (ifs_is_active(&ctx->ifs)) {
      fputc('#', out);
      fputs(line_raw, out); // incluye \n si estaba
    }
    return 0;
  }
    /* Parsear la línea de la directiva */
  int col = 1;
  int line_no = (*linea_actual) - 1; // porque ya incrementamos al leer fgets
  if (line_no <= 0) line_no = *linea_actual;

  /* Parsear la línea con el módulo de guardar_directivas:
     - line_after_hash = line_raw sin el \n .
     - Le pasamos el texto tal cual después del '#'.
  */
  Directiva d;
  memset(&d, 0, sizeof(d));

  GDError gd_err;
  memset(&gd_err, 0, sizeof(gd_err));

//   Lógica de parseo (hay que implementar guardar_directivas del modulo de guardar_directivas)

//   if (guardar_directivas_parse_line(ctx->current_file, line_no, col, line_raw, &d, &gd_err) != 0) {
//     SrcLoc loc = { ctx->current_file, line_no, col };
//     ds_set_error(err, loc, gd_err.msg[0] ? gd_err.msg : "directive parse error");
//     return 1;
//   }

  /* Aplicar directiva:*/
  const Directiva *dp = &d;

  switch (dp->kind) {

    case DIR_IFDEF: {
      // Siempre hay que actualizar el stack, incluso si estamos en bloque inactivo,
      // porque necesitamos poder cerrar con #endif correctamente.
      //int cond_true = evaluar si la macro está definida
      if (ifs_push(&ctx->ifs, cond_true) != 0) {
        SrcLoc loc = { ctx->current_file, line_no, col };
        ds_set_error(err, loc, "ifdef nesting too deep");
        if (!optional_log) directiva_free(&d);
        return 1;
      }
    } break;

    case DIR_ENDIF: {
      if (ifs_pop(&ctx->ifs) != 0) {
        SrcLoc loc = { ctx->current_file, line_no, col };
        ds_set_error(err, loc, "endif without ifdef");
        if (!optional_log) directiva_free(&d);
        return 1;
      }
    } break;

    case DIR_DEFINE: {
      // Sólo aplicamos define si estamos activos
      if (ifs_is_active(&ctx->ifs)) {
        // Para P1PP simple: sin parámetros

        // Guardar macro en la tabla (ponemos la función guardar_macro)
      
        // la línea #define NO se copia al output (directiva soportada)
    } break;

    case DIR_INCLUDE: {
      // Sólo aplicamos include si estamos activos
      if (ifs_is_active(&ctx->ifs)) {
        if (!process_file_fn) {
          // si no tenemos callback, degradamos: copiar la directiva tal cual para no romper
          fputc('#', out);
          fputs(line_raw, out);
        } else {
          // Procesamiento recursivo del archivo incluido
          if (process_file_fn(dp->as.inc.path, out, flags, ctx, err) != 0) {
            // err ya debe venir seteado por process_file_fn
            directiva_free(&d);
            return 1;
          }
        }
      }
      // si bloque inactivo: ignoramos include
    } break;

    case DIR_UNKNOWN:
    default: {
      // Directiva no soportada -> se copia tal cual (si activo), como pide el enunciado
      if (ifs_is_active(&ctx->ifs)) {
        fputc('#', out);
        fputs(line_raw, out);
      }
    } break;
  }

  /* Liberación si NO la guardamos en lista */
  directiva_free(&d);

  // OK
  SrcLoc ok = { ctx->current_file, line_no, col };
  ds_set_error(err, ok, "OK");
  return 0;
}
}