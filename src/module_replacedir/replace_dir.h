#ifndef DIRECTIVE_SUBSTITUTION_H
#define DIRECTIVE_SUBSTITUTION_H

#include "directivas_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Evitar "magic chars/strings": centralizamos constantes */
#define DS_CH_NL '\n'
#define DS_CH_CR '\r'
#define DS_CH_SLASH '/'
#define DS_CH_BSLASH '\\'
#define DS_STR_NL "\n"

/* Callback de lectura de ficheros (lo implementará el módulo de ficheros en el futuro).
   - Retorna 0 si OK, !=0 si error.
   - out_content debe ser malloc() y lo libera quien lo recibe.
   - err_msg opcional para mensaje humano. */
typedef int (*DSReadFileFn)(const char *path,
                            char **out_content,
                            int *out_len,
                            char *err_msg,
                            int err_msg_cap);

typedef struct {
  int recursive_include;          /* 0 = shallow (incluye texto sin preprocesar), 1 = deep (NO implementado aquí) */
  const char *include_path_base;  /* base para includes relativos (puede ser NULL) */
  int max_expansion_depth;        /* seguridad futura (si se implementa deep) */
  int max_output_bytes;           /* 0 = sin límite (recomendado poner algo) */
} DSConfig;

typedef struct {
  SrcLoc loc;
  char msg[256];
} DSError;

/* API principal del módulo */
int directive_substitution_apply(const char *source_content,
                                 const DirectivaList *directives,
                                 const DSConfig *cfg,
                                 DSReadFileFn read_file,     /* puede ser NULL -> fallback fopen temporal */
                                 char **out_result,
                                 DSError *err);

#ifdef __cplusplus
}
#endif

#endif
