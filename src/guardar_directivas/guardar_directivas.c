#include "./guardar_directivas.h"

#include <stdio.h>
#include <string.h>

extern FILE *ofile;

static void print_arguments(int argc, char *argv[]) {
  fprintf(ofile, "guardar_directivas :: Arguments received (%d):\n", argc);
  for (int i = 0; i < argc; i++) {
    fprintf(ofile, "  argv[%d] = %s\n", i, argv[i]);
  }
  fflush(ofile);
}

static void gd_set_error(GDError *err, const char *file, int line, int col, const char *msg) {
  if (!err) return;
  err->loc.file = file;
  err->loc.line = line;
  err->loc.col  = col;
  strncpy(err->msg, msg, sizeof(err->msg));
  err->msg[sizeof(err->msg) - 1] = '\0';
}

int guardar_directivas_parse_file(const char *path, DirectivaList *out,
                                 const GDConfig *cfg, GDError *err) {
  (void)cfg;

  if (!path || !out) {
    gd_set_error(err, "<no-file>", 0, 0, "Invalid arguments");
    return 1;
  }

  // inicializamos la lista pero todavía no parseamos nada.
  directivalist_init(out);

  // mas adelante aquí irá el parser real con:
  // - fopen(path), leer char-a-char, detectar '#' al inicio de línea lógica (lo hace preprocesador), crear Directiva (struct) y directivalist_push(...), cerrar file
  gd_set_error(err, path, 0, 0, "OK (stub): parser not implemented yet");
  return 0;
}

int process_guardar_directivas(int argc, char *argv[]) {
  print_arguments(argc, argv);

  fprintf(ofile, "Module guardar_directivas: not implemented yet\n");

  // demo de API sin parse real
  if (argc >= 2) {
    GDConfig cfg = { .max_directives = 10000, .max_token_len = 256 };
    GDError err;
    DirectivaList list;

    int rc = guardar_directivas_parse_file(argv[1], &list, &cfg, &err);
    fprintf(ofile, "guardar_directivas_parse_file('%s') => rc=%d, msg='%s'\n",
            argv[1], rc, err.msg);

    directivalist_free(&list);
  } else {
    fprintf(ofile, "Usage (module test): <program> <input_file>\n");
  }

  fflush(ofile);
  return 0;
}
