/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: directivas_types.c
* Autor: Wassim Ibn Tahar
* Fecha: 18/01/2026
*
* Descripción:
* Este módulo implementa la liberación de memoria dinámica asociada a la Directiva. 
* Dado que los datos específicos de cada directiva (nombre, valor, ruta, etc.) se almacenan mediante una unión,
* esta función se encarga de liberar únicamente los campos válidos según el tipo de directiva (DirKind).
*
* Despues de liberar la memoria, la estructura se deja en un estado seguro reiniciando el tipo a DIR_UNKNOWN y anulando los punteros internos
* -----------------------------------------------------------------------------
*/

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
