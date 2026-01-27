/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: directivas_types.h
* Autor: Wassim Ibn Tahar
* Fecha: 18/01/2026
*
* Descripción:
* Este archivo define los tipos de datos usados para representar directivas del
* preprocesador de forma estructurada. Incluye:
*   - DirKind: enumeración con los tipos de directiva soportados (#define, #include,
*     #ifdef, #endif) y un tipo DIR_UNKNOWN para cualquier otra directiva.
*   - Directiva: estructura que almacena el tipo de directiva, su localización
*     (SrcLoc) y los datos asociados mediante una unión para cada caso
*     (nombre/valor, ruta de include, símbolo de ifdef o texto crudo).
*     Hacemos union ya que la directiva solo puede ser de un tipo a la vez y optimizamos el uso de memoria
*
* También declara la función directiva_free(), encargada de liberar la memoria
* dinámica asociada a una Directiva
* -----------------------------------------------------------------------------
*/

#ifndef DIRECTIVAS_TYPES_H
#define DIRECTIVAS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../module_preprocessor/pp_shared.h"


//Tipo de directiva detectada
typedef enum {
  DIR_DEFINE,
  DIR_INCLUDE,
  DIR_IFDEF,

  DIR_ENDIF,
  DIR_UNKNOWN
} DirKind;



typedef struct {
  DirKind kind;
  SrcLoc loc;

  union {
    struct {               // #define NAME value...
      char *name;
      char *value;         // string cruda (puede ser "")
    } def;

    struct {               //#include "file"
      char *path;
    } inc;

    struct {               //#ifdef NAME
      char *name;
    } ifdef;

    struct {               //#unknown
      char *raw;
    } unknown;
  } as;
} Directiva;


void directiva_free(Directiva *d);

#ifdef __cplusplus
}
#endif

#endif
