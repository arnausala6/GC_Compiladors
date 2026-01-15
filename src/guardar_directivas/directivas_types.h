#ifndef DIRECTIVAS_TYPES_H
#define DIRECTIVAS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

// Tipo de directiva detectada
typedef enum {
  DIR_DEFINE,
  DIR_UNDEF,
  DIR_INCLUDE,
  DIR_IFDEF,
  DIR_IFNDEF,
  DIR_IF,
  DIR_ELIF,
  DIR_ELSE,
  DIR_ENDIF,
  DIR_UNKNOWN
} DirKind;

// Localización para errores y debug
typedef struct {
  const char *file;
  int line;
  int col;
} SrcLoc;

// Cómo se escribió el include
typedef enum {
  INC_QUOTED,   // "file.h"
  INC_ANGLED,   // <file.h>
  INC_UNKNOWN
} IncludeKind;

// Una directiva parseada (pero NO ejecutada)
typedef struct {
  DirKind kind;
  SrcLoc loc;

  union {
    struct {               // #define NAME value...
      char *name;
      char *value;         // string cruda (puede ser "")
    } def;

    struct {               // #undef NAME
      char *name;
    } undefn;

    struct {               // #include "x" o <x>
      IncludeKind style;
      char *path;
    } inc;

    struct {               // #ifdef NAME / #ifndef NAME
      char *name;
    } ifdef;

    struct {               // #if expr / #elif expr (expr cruda)
      char *expr;
    } ifexpr;

    struct {               // #unknown ...
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

// Gestión de lista minimo (mejorar)
void directivalist_init(DirectivaList *l);
int  directivalist_push(DirectivaList *l, Directiva d);
void directivalist_free(DirectivaList *l);

// Free de una directiva individual (útil para el resto de módulos)
void directiva_free(Directiva *d);

#ifdef __cplusplus
}
#endif

#endif
