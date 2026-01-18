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
int guardar_directivas_parse_file(const char *path, DirectivaList *out, GDError *err);
                                 
#ifdef __cplusplus
}
#endif

#endif
