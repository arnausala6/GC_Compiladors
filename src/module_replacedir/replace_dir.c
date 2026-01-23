#include "replace_dir.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int set_err(GDError *err, SrcLoc loc, const char *msg) {
  if (!err) return 1;
  err->loc = loc;
  strncpy(err->msg, msg, sizeof(err->msg) - 1);
  err->msg[sizeof(err->msg) - 1] = '\0';
  return 0;
}

/*IfStack*/
int ifs_init(IfStack *st) {
  if (!st) return 1;
  st->depth = 0;
  st->active[0] = 1; // nivel 0 activo
  return 0;
}

int ifs_is_active(const IfStack *st) {
  if (!st) return 1; // si no hay stack, asumimos activo
  if (st->depth < 0 || st->depth >= DS_MAX_IF_DEPTH) return 0;
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

/* Función principal */

int replace_directives_handle_hash(
  FILE *in, FILE *out, int *lineactual, Tabla_macros *macros, GDError *err, IfStack *ifstack
) {
  if (!d || !out || !macros || !err || !ifstack) {
    return 1;
  }

  char restline[2048];
  if (fgets(restline, sizeof(restline), in) == NULL) {
    return set_err(err, d->loc, "failed to read directive line");
  }

  Directiva d;
  memset(&d, 0, sizeof(d));
  if (guardar_directiva_parse_line(restline, &d, err) != 0) {
    // err ya seteado
    return 1;
  }

  (*lineactual)++;
  switch (d.kind) {
    case DIR_DEFINE:
      if (!ifs_is_active(ifstack)) break;

      if (!d.as.def.name || d.as.def.name[0] == '\0')
        return set_err(err, d.loc, "define without name");

      const char *val = d.as.def.value ? d.as.def.value : "";

      if (guardar_macro(macros, d.as.def.name, val) != 0)
        return set_err(err, d.loc, "failed to store macro");

      break;

    case DIR_UNDEF:
      if (ifs_is_active(ifstack)) {
        //tabla_macros_remove(macros, d->as.undefn.name);
      }
      break;

    case DIR_IFDEF: {
      if (!d.as.ifdef.name || d.as.ifdef.name[0] == '\0'){
        directiva_free(&d);
        return set_err(err, d.loc, "ifdef without name");
      }

      if (!ifs_is_active(ifstack)) {
        if (ifs_push(ifstack, 0) != 0) {
          directiva_free(&d);
          return set_err(err, d.loc, "ifdef nesting too deep");
        }
        break;
      }
      int is_defined = tabla_macros_exists(macros, d.as.ifdef.name);

      if (ifs_push(ifstack, is_defined) != 0) {
        directiva_free(&d);
        return set_err(err, d.loc, "ifdef nesting too deep");
      }

      break;
    }

    case DIR_IFNDEF: {
      if (!d.as.ifdef.name || d.as.ifdef.name[0] == '\0') {
        directiva_free(&d);
        return set_err(err, d.loc, "ifndef without name");
      }

      if (!ifs_is_active(ifstack)) {
        if (ifs_push(ifstack, 0) != 0) {
          directiva_free(&d);
          return set_err(err, d.loc, "ifndef nesting too deep");
        }
        break;
      }

      int is_defined = tabla_macros_exists(macros, d.as.ifdef.name);
      if (ifs_push(ifstack, !is_defined) != 0) {
        directiva_free(&d);
        return set_err(err, d.loc, "ifndef nesting too deep");
      }
      break;
    }

    case DIR_ENDIF:
      if (ifs_pop(ifstack) != 0) {
        directiva_free(&d);
        return set_err(err, d.loc, "endif without matching ifdef/ifndef");
      }
      break;
    
    case DIR_INCLUDE:
      // Manejo de includes falta por implementar
      break;

    default:
      // Otras directivas no manejadas aquí
      break;
  }
  directiva_free(&d);
  return 0;
}