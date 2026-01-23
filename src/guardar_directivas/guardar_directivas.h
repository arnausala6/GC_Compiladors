#ifndef GUARDAR_DIRECTIVAS_H
#define GUARDAR_DIRECTIVAS_H

#include "directivas_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  SrcLoc loc;
  char msg[256];
} GDError;

//util para otros módulos
int guardar_directiva_parse_line(const char *line, SrcLoc loc, Directiva *out, GDError *err);
                                 
#ifdef __cplusplus
}
#endif

#endif
