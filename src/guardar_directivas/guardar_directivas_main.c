#include "guardar_directivas.h"
#include <stdio.h>

FILE *ofile = NULL;

static const char *dirkind_to_str(DirKind k) {
  switch (k) {
    case DIR_DEFINE:   return "DIR_DEFINE";
    case DIR_INCLUDE:  return "DIR_INCLUDE";
    case DIR_IFDEF:    return "DIR_IFDEF";
    case DIR_ENDIF:    return "DIR_ENDIF";
    case DIR_UNKNOWN:  return "DIR_UNKNOWN";
    default:           return "DIR_???";
  }
}

int main(int argc, char *argv[]) {
  ofile = stdout;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  const char *path = argv[1];


  GDError err;
  DirectivaList list;

  int rc = guardar_directivas_parse_file(path, &list, &err);
  if (rc != 0) {
    fprintf(stderr, "Error at %s:%d:%d -> %s\n",
            err.loc.file ? err.loc.file : "<unknown>",
            err.loc.line,
            err.loc.col,
            err.msg);
    return 2;
  }

  printf("Total directives: %d\n\n", list.size);

  for (int i = 0; i < list.size; i++) {
    Directiva *d = &list.data[i];
    printf("Directive #%d\n", i);
    printf("  kind: %s\n", dirkind_to_str(d->kind));
    printf("  loc : %s:%d:%d\n",
           d->loc.file ? d->loc.file : "<unknown>",
           d->loc.line,
           d->loc.col);

    switch (d->kind) {
      case DIR_DEFINE:
        printf("  name : %s\n", d->as.def.name);
        printf("  value: %s\n", d->as.def.value);
        break;
      case DIR_INCLUDE:
        printf("  path : %s\n", d->as.inc.path);
        break;
      case DIR_IFDEF:
        printf("  name : %s\n", d->as.ifdef.name);
        break;
      case DIR_UNKNOWN:
        printf("  raw  : %s\n", d->as.unknown.raw);
        break;
      default:
        break;
    }

    printf("\n");
  }

  directivalist_free(&list);
  return 0;
}
//ir a guardar_directiva, comandos rm -rf build y mkdir build, y cd build, cmake .., cmake --build ., ./test_guardar_directivas ../test.c