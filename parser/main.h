#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Flags de terminal --------------------------------------------------- */
#define FLAG_INPUT  "-i"
#define FLAG_LANG   "-lang"
#define FLAG_MODE   "-mode"
#define FLAG_TRACE  "-trace"
#define FLAG_COUNT  "-count"
#define VAL_RELEASE "release"
#define VAL_DEBUG   "debug"

/* --- Configuración de la aplicación -------------------------------------- */

typedef enum {
    OUTFORMAT_RELEASE,
    OUTFORMAT_DEBUG
} OutFormat;

typedef struct {
    OutFormat outformat;
    int debug_on;
    int count_enabled;
    int countout;
} AppConfig;

extern FILE *ofile;

int main(int argc, char **argv);

#endif /* MAIN_H */
