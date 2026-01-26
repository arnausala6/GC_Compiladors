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
