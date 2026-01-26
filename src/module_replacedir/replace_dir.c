#include "replace_dir.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int set_err(GDError *err, SrcLoc loc, const char *msg) {
  if (!err) return 1;
  err->loc = loc;
  strncpy(err->msg, msg, sizeof(err->msg) - 1);
  err->msg[sizeof(err->msg) - 1] = '\0';
  return 1;
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

static int build_include_path(char *dst, size_t dst_sz,
                              const char *current_file,
                              const char *inc_path) {
  if (!inc_path || !inc_path[0]) return 1;

  if (!current_file) {
    if (snprintf(dst, dst_sz, "%s", inc_path) >= (int)dst_sz) return 1;
    else { return 0; }
  }

  const char *slash = strrchr(current_file, '/');
  if (!slash) {
    if (snprintf(dst, dst_sz, "%s", inc_path) >= (int)dst_sz) return 1;
    else { return 0; }
  }

  size_t dirlen = (size_t)(slash - current_file + 1); // incluye '/'
  if (dirlen + strlen(inc_path) + 1 > dst_sz) return 1;

  memcpy(dst, current_file, dirlen);
  strcpy(dst + dirlen, inc_path);
  return 0;
}


/* Función principal */

int replace_directives_handle_hash(
  FILE *in, FILE *out, int *lineactual, int flags, Tabla_macros *macros, GDError *err, IfStack *ifstack, const char *current_file
) {
  if (!in || !out || !lineactual || !macros || !err || !ifstack) {
    return 1;
  }

  char restline[MAX_DIRECTIVE_LINE_LENGTH];
  if (fgets(restline, sizeof(restline), in) == NULL) {
    SrcLoc loc = {0};
    loc.file = current_file;   
    loc.line = *lineactual;
    loc.col  = 1;
    return set_err(err, loc, "failed to read directive line");
  }

  Directiva d;
  memset(&d, 0, sizeof(d));
  SrcLoc loc = {0};
  loc.file = current_file;   
  loc.line = *lineactual;
  loc.col  = 1;
  if (guardar_directiva_parse_line(restline, loc, &d, err) != 0) {
    // err ya seteado
    return 1;
  }

  (*lineactual)++; // avanzamos línea
  switch (d.kind) {
    case DIR_DEFINE:{
      if (!ifs_is_active(ifstack)) break;

      if (!d.as.def.name || d.as.def.name[0] == '\0'){
        directiva_free(&d);
        return set_err(err, d.loc, "define without name");
      }

      const char *val = d.as.def.value ? d.as.def.value : "";

      guardar_macro(macros, d.as.def.name, val);
      break;
    }
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
      
      // Verificar si la macro está definida
      int is_defined = 0;
      for (int i = 0; i < macros->elementos; i++) {
        Macro *m = macros->macros[i];
        if (strcmp(d.as.ifdef.name, m->nombre) == 0) {
          is_defined = 1;
        }
      }
      if (ifs_push(ifstack, is_defined) != 0) {
        directiva_free(&d);
        return set_err(err, d.loc, "ifdef nesting too deep");
      }

      break;
    }

    case DIR_ENDIF: {
      if (ifs_pop(ifstack) != 0) {
        directiva_free(&d);
        return set_err(err, d.loc, "endif without matching ifdef/ifndef");
      }
      break;
    }

    case DIR_INCLUDE:{
      if (!ifs_is_active(ifstack)) break;
      int rc;
      if (!d.as.inc.path || d.as.inc.path[0] == '\0') {
        rc = set_err(err, d.loc, "include without path");
        directiva_free(&d);
        return rc;
      }
      char fullpath[MAX_INCLUDE_PATH_LENGTH];
      if (build_include_path(fullpath, sizeof(fullpath), current_file, d.as.inc.path) != 0) {
        rc = set_err(err, d.loc, "include path too long / invalid");
        directiva_free(&d);
        return rc;
      } 
      FILE *incfile = fopen(fullpath, "r");
      if (!incfile) {
        rc = set_err(err, d.loc, "failed to open include file");
        directiva_free(&d);
        return rc;
      }
      int incl_lineactual = 1;
      rc = motor_preprocesador(incfile, out, flags, fullpath, macros, ifstack, err);
      fclose(incfile);
      if (rc != 0) {
        directiva_free(&d);
        return rc;
      }
    }
      break;

    default:
      // Otras directivas no manejadas aquí, poner tal cual
      fprintf(out, "#%s", restline);
      break;
  }
  directiva_free(&d);
  return 0;
}