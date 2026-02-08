#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils_files.h"

// --- Definiciones de Constantes y Flags ---
#define PROJOUTFILENAME "./proj_modules_template.log"

// Flags de terminal esperadas
#define FLAG_INPUT      "-i"         // Archivo de entrada
#define FLAG_MODE       "-mode"      // release vs debug (OUTFORMAT)
#define FLAG_TRACE      "-trace"     // Activa mensajes de debug (DEBUG=1)
#define FLAG_COUNT      "-count"     // Activa conteo de operaciones (COUNTCONFIG)
#define VAL_RELEASE     "release"
#define VAL_DEBUG       "debug"


typedef enum {
  OUTFORMAT_RELEASE,
  OUTFORMAT_DEBUG
} OutFormat;

typedef struct {
  OutFormat outformat;
  int debug_on;        // DEBUG = 1 / 0
  int count_enabled;   // COUNTCONFIG
  int countout;        // COUNTOUT (0 = file, 1 = stdout)
} AppConfig;

typedef struct {
  const char *input_path;
  FILE *input_file;
  FILE *output_file;   // .cscn
  FILE *dbgcnt_file;   // .dbgcnt
} FileCtx;

extern FILE* ofile; 

#endif // MAIN_H