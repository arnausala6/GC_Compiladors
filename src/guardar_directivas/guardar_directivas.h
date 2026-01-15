#ifndef GUARDAR_DIRECTIVAS_H
#define GUARDAR_DIRECTIVAS_H

#include "directivas_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int max_directives;   // seguridad
  int max_token_len;    // para buffers al parsear
} GDConfig;

typedef struct {
  SrcLoc loc;
  char msg[256];
} GDError;

// útil para otros módulos:
// Lee caracter a caracter y guarda directivas en out (de momento todavía stub).
int guardar_directivas_parse_file(const char *path, DirectivaList *out,
                                 const GDConfig *cfg, GDError *err);
                                 
int process_guardar_directivas(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif
