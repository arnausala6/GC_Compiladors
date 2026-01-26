#include "directivas_types.h"
#include <stdlib.h>

void directiva_free(Directiva *d) {
  if (!d) return;

  switch (d->kind) {
    case DIR_DEFINE:
      free(d->as.def.name);
      free(d->as.def.value);
      break;
    case DIR_INCLUDE:
      free(d->as.inc.path);
      break;
    case DIR_IFDEF:
      free(d->as.ifdef.name);
      break;
    case DIR_UNKNOWN:
    default:
      free(d->as.unknown.raw);
      break;
  }

  // limpia punteros
  d->kind = DIR_UNKNOWN;
  d->as.unknown.raw = NULL;
}
