#include "guardar_directivas.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//Guarda un error con localización y mensaje
static void gd_set_error(GDError *err, SrcLoc loc, const char *msg) {
  if (!err) return;
  err->loc = loc;
  strncpy(err->msg, msg ? msg : "", sizeof(err->msg));
  err->msg[sizeof(err->msg) - 1] = '\0';
}

//duplica un string en memoria dinámica
static char *dupstr(const char *s) {
  if (!s) return NULL;
  size_t n = strlen(s);
  char *p = (char*)malloc(n + 1);
  if (!p) return NULL;
  memcpy(p, s, n + 1);
  return p;
}

//salta espacios y tabs por la izquierda 
static char *skip_spaces(char *s) {
  while (*s && isspace((unsigned char)*s)) s++;
  return s;
}

// quita \n / \r del final
static void chomp(char *s) {
  if (!s) return;
  s[strcspn(s, "\r\n")] = '\0';
}

/* separa primer token: pone '\0' en el primer espacio */
static char *split_token(char *s) {
  while (*s && !isspace((unsigned char)*s)) s++;
  if (*s) { *s = '\0'; s++; }
  return s;
}

int guardar_directiva_parse_line(const char *line, SrcLoc loc, Directiva *out, GDError *err) {
  if (!line || !out) {
    gd_set_error(err, loc, "bad args");
    return 1;
  }

  memset(out, 0, sizeof(*out));
  out->loc = loc;
  out->kind = DIR_UNKNOWN;

  // copiamos a buffer editable
  char buf[2048];
  strncpy(buf, line, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';
  chomp(buf);

  char *p = skip_spaces(buf);

  p++; // salto '#'
  p = skip_spaces(p);
  if (*p == '\0') {
    // línea "#" sola => no directiva (puedes tratarlo como unknown vacío o ignorarlo)
    out->kind = DIR_UNKNOWN;
    out->as.unknown.raw = dupstr("");
    if (!out->as.unknown.raw) { gd_set_error(err, loc, "out of memory"); return 1; }
    return 0;
  }

  char *kw = p;
  p = split_token(p);
  char *args = skip_spaces(p);

  if (strcmp(kw, "define") == 0) {
    out->kind = DIR_DEFINE;

    char *name = args;
    char *rest = split_token(args);
    rest = skip_spaces(rest);

    if (!name || *name == '\0') {
      gd_set_error(err, loc, "define without name");
      return 1;
    }

    out->as.def.name  = dupstr(name);
    out->as.def.value = dupstr(rest ? rest : "");

    if (!out->as.def.name || !out->as.def.value) {
      directiva_free(out);
      gd_set_error(err, loc, "out of memory");
      return 1;
    }

  } else if (strcmp(kw, "include") == 0) {
    args = skip_spaces(args);

    if (*args == '"') {
      out->kind = DIR_INCLUDE;

      args++;
      char *end = strchr(args, '"');
      if (end) *end = '\0';

      out->as.inc.path = dupstr(args);
      if (!out->as.inc.path) {
        directiva_free(out);
        gd_set_error(err, loc, "out of memory");
        return 1;
      }
    } else {
      // include <...> u otros -> unknown (para que el sustituidor decida “imprimir tal cual”)
      out->kind = DIR_UNKNOWN;

      char tmp[2048];
      if (*args) snprintf(tmp, sizeof(tmp), "include %s", args);
      else       snprintf(tmp, sizeof(tmp), "include");

      out->as.unknown.raw = dupstr(tmp);
      if (!out->as.unknown.raw) {
        directiva_free(out);
        gd_set_error(err, loc, "out of memory");
        return 1;
      }
    }

  } else if (strcmp(kw, "ifdef") == 0) {
    out->kind = DIR_IFDEF;

    char *name = args;
    split_token(args);

    if (!name || *name == '\0') {
      gd_set_error(err, loc, "ifdef without name");
      return 1;
    }

    out->as.ifdef.name = dupstr(name);
    if (!out->as.ifdef.name) {
      directiva_free(out);
      gd_set_error(err, loc, "out of memory");
      return 1;
    }

  } else if (strcmp(kw, "endif") == 0) {
    out->kind = DIR_ENDIF;

  } else {
    out->kind = DIR_UNKNOWN;

    if (args && *args) {
      char tmp[2048];
      snprintf(tmp, sizeof(tmp), "%s %s", kw, args);
      out->as.unknown.raw = dupstr(tmp);
    } else {
      out->as.unknown.raw = dupstr(kw);
    }

    if (!out->as.unknown.raw) {
      directiva_free(out);
      gd_set_error(err, loc, "out of memory");
      return 1;
    }
  }

  gd_set_error(err, loc, "OK");
  return 0;
}
