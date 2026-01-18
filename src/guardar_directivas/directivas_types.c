#include "directivas_types.h"
#include <stdlib.h>

void directivalist_init(DirectivaList *l) {
  l->data = NULL;
  l->size = 0;
  l->cap  = 0;
}

int directivalist_push(DirectivaList *l, Directiva d) {
  if (!l) return 0;

  if (l->size == l->cap) {
    int new_cap = (l->cap == 0) ? 8 : l->cap * 2;
    void *p = realloc(l->data, (size_t)new_cap * sizeof(Directiva));
    if (!p) return 0;
    l->data = (Directiva*)p;
    l->cap  = new_cap;
  }

  l->data[l->size++] = d;
  return 1;
}

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

void directivalist_free(DirectivaList *l) {
  if (!l) return;
  for (int i = 0; i < l->size; i++) {
    directiva_free(&l->data[i]);
  }
  free(l->data);
  l->data = NULL;
  l->size = 0;
  l->cap  = 0;
}
