#include "guardar_directivas.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//Guarda un error con localización y mensaje
static void gd_set_error(GDError *err, const char *file, int line, int col, const char *msg) {
  if (!err) return;
  err->loc.file = file;
  err->loc.line = line;
  err->loc.col  = col;
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

//Lee un fichero línea a línea y guarda las directivas del preprocesador
int guardar_directivas_parse_file(const char *path, DirectivaList *out,
                                  GDError *err) {

  if (!path || !out) {
    gd_set_error(err, "<no-file>", 0, 0, "bad args");
    return 1;
  }

  FILE *f = fopen(path, "r");
  if (!f) {
    gd_set_error(err, path, 0, 0, "cannot open file");
    return 1;
  }

  directivalist_init(out);

  char line[2048];
  int line_no = 0;

  while (fgets(line, sizeof(line), f)) {
    line_no++;
    chomp(line);

    char *p = skip_spaces(line);
    if (*p != '#') continue;

    int col = (int)(p - line) + 1;

    p++; /* salto '#' */
    p = skip_spaces(p);
    if (*p == '\0') continue;

    /* keyword */
    char *kw = p;
    p = split_token(p);
    char *args = skip_spaces(p);

    Directiva d;
    memset(&d, 0, sizeof(d));
    d.loc.file = path;
    d.loc.line = line_no;
    d.loc.col  = col;
    d.kind = DIR_UNKNOWN;

    if (strcmp(kw, "define") == 0) {
      /* #define NAME value...*/
      d.kind = DIR_DEFINE;

      char *name = args;
      char *rest = split_token(args);
      rest = skip_spaces(rest);

      d.as.def.name  = dupstr(name);
      d.as.def.value = dupstr(rest);

      if (!d.as.def.name || !d.as.def.value) {
        directiva_free(&d);
        directivalist_free(out);
        fclose(f);
        gd_set_error(err, path, line_no, col, "out of memory");
        return 1;
      }

    } else if (strcmp(kw, "include") == 0) {
      // solo #include "file"
      args = skip_spaces(args);

      if (*args == '"') {
        d.kind = DIR_INCLUDE;

        args++; // salto la comilla inicial
        char *end = strchr(args, '"');
        if (end) *end = '\0';

        d.as.inc.path = dupstr(args);
        if (!d.as.inc.path) {
          directiva_free(&d);
          directivalist_free(out);
          fclose(f);
          gd_set_error(err, path, line_no, col, "out of memory");
          return 1;
        }

      } else {
        // include no soportado como <stdio.h> lo guardo como unknown
        d.kind = DIR_UNKNOWN;

        char buf[2048];
        if (*args) snprintf(buf, sizeof(buf), "include %s", args);
        else       snprintf(buf, sizeof(buf), "include");

        d.as.unknown.raw = dupstr(buf);
        if (!d.as.unknown.raw) {
          directiva_free(&d);
          directivalist_free(out);
          fclose(f);
          gd_set_error(err, path, line_no, col, "out of memory");
          return 1;
        }
      }

    } else if (strcmp(kw, "ifdef") == 0) {
      d.kind = DIR_IFDEF;

      char *name = args;
      split_token(args); //corta el primer token
      d.as.ifdef.name = dupstr(name);

      if (!d.as.ifdef.name) {
        directiva_free(&d);
        directivalist_free(out);
        fclose(f);
        gd_set_error(err, path, line_no, col, "out of memory");
        return 1;
      }

    } else if (strcmp(kw, "endif") == 0) {
      d.kind = DIR_ENDIF;

    } else {
      //directiva desconocida
      d.kind = DIR_UNKNOWN;

      if (args && *args) {
        char buf[2048];
        snprintf(buf, sizeof(buf), "%s %s", kw, args);
        d.as.unknown.raw = dupstr(buf);
      } else {
        d.as.unknown.raw = dupstr(kw);
      }

      if (!d.as.unknown.raw) {
        directiva_free(&d);
        directivalist_free(out);
        fclose(f);
        gd_set_error(err, path, line_no, col, "out of memory");
        return 1;
      }
    }

    if (!directivalist_push(out, d)) {
      directiva_free(&d);
      directivalist_free(out);
      fclose(f);
      gd_set_error(err, path, line_no, col, "push failed");
      return 1;
    }
  }

  fclose(f);
  gd_set_error(err, path, 0, 0, "OK");
  return 0;
}