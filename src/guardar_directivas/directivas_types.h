#ifndef DIRECTIVAS_TYPES_H
#define DIRECTIVAS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

//Tipo de directiva detectada
typedef enum {
  DIR_DEFINE,
  DIR_INCLUDE,
  DIR_IFDEF,

  DIR_ENDIF,
  DIR_UNKNOWN
} DirKind;

//localizacion para errores y debug
typedef struct {
  const char *file;
  int line;
  int col;
} SrcLoc;

//el include
typedef enum {
  INC_QUOTED,   //"file.h"
  INC_UNKNOWN
} IncludeKind;

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

// Lista dinámica de directivas
typedef struct {
  Directiva *data;
  int size;
  int cap;
} DirectivaList;

//gestión de lista
void directivalist_init(DirectivaList *l);
int  directivalist_push(DirectivaList *l, Directiva d);
void directivalist_free(DirectivaList *l);

void directiva_free(Directiva *d);

#ifdef __cplusplus
}
#endif

#endif
