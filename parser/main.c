/*
 * Programa: P3 – Bottom-up Parsing (Shift-Reduce Automaton)
 * Archivo:  main.c
 *
 * Descripción:
 *   Driver principal del parser.  Orquesta la carga del fichero de lenguaje
 *   (producciones + tablas ACTION/GOTO), la carga del flujo de tokens (.cscn),
 *   la apertura del fichero de traza, la inicialización del motor SRA, la
 *   ejecución del análisis y la limpieza de recursos.
 *
 *   No contiene lógica de parsing; toda la lógica reside en sra_engine.
 */

#include "main.h"
#include "module_args/module_args.h"
#include "language/language.h"
#include "modulo_tokenlist/tokenlist.h"
#include "sra_engine/sra_engine.h"

#define PROJOUTFILENAME "stdout"

FILE *ofile = NULL;

static int extract_lang_path(int argc, char *argv[], char **lang_path)
{
    *lang_path = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], FLAG_LANG) == 0) {
            if (i + 1 < argc) {
                *lang_path = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requiere un fichero.\n", FLAG_LANG);
                return 1;
            }
        }
    }
    if (*lang_path == NULL) {
        fprintf(stderr,
                "Error: No se ha especificado fichero de lenguaje (%s).\n",
                FLAG_LANG);
        return 1;
    }
    return 0;
}

/*
 * <input>.cscn  ->  <input>_p3dbg.txt
 * <input>       ->  <input>_p3dbg.txt   (si no tiene extensión .cscn)
 */
static int build_dbg_path(const char *input, char *out, size_t max_len)
{
    const char *suffix  = "_p3dbg.txt";
    const char *ext     = ".cscn";
    size_t ext_len = strlen(ext);
    size_t in_len  = strlen(input);

    if (in_len >= ext_len &&
        strcmp(input + in_len - ext_len, ext) == 0) {
        size_t base = in_len - ext_len;
        if (base + strlen(suffix) >= max_len) return 1;
        memcpy(out, input, base);
        strcpy(out + base, suffix);
    } else {
        if (in_len + strlen(suffix) >= max_len) return 1;
        strcpy(out, input);
        strcat(out, suffix);
    }
    return 0;
}

int main(int argc, char **argv)
{
    ofile = set_output_test_file(PROJOUTFILENAME);

    /* 1. Procesar argumentos */
    AppConfig config;
    char *input_filename = NULL;
    char *lang_path      = NULL;

    if (extract_lang_path(argc, argv, &lang_path) != 0)
        return 1;
    if (process_arguments(argc, argv, &config, &input_filename) != 0)
        return 1;

    fprintf(ofile, "[MAIN] Input: %s | Language: %s | Mode: %d\n",
            input_filename, lang_path, config.outformat);

    /* 2. Cargar especificación del lenguaje */
    Language lang;
    if (language_load(&lang, lang_path) != 0) {
        fprintf(stderr,
                "Error: no se pudo cargar el fichero de lenguaje '%s'.\n",
                lang_path);
        return 1;
    }

    /* 3. Cargar flujo de tokens (.cscn) */
    TokenStream ts;
    if (tokenlist_load(&ts, input_filename) != 0) {
        fprintf(stderr,
                "Error: no se pudo cargar el fichero de tokens '%s'.\n",
                input_filename);
        return 1;
    }

    /* 4. Abrir fichero de traza */
    char dbg_path[512];
    if (build_dbg_path(input_filename, dbg_path, sizeof(dbg_path)) != 0) {
        fprintf(stderr, "Error: ruta de entrada demasiado larga.\n");
        return 1;
    }

    FILE *dbg_out = fopen(dbg_path, "w");
    if (!dbg_out) {
        perror("Error abriendo fichero de traza");
        return 1;
    }
    fprintf(ofile, "[MAIN] Fichero de traza: %s\n", dbg_path);

    /* 5. Inicializar y ejecutar motor SRA */
    init_sra_engine(&lang.engine, &ts, dbg_out);
    run_sra_engine(&lang);

    // if (result == 0)
    //     fprintf(ofile, "[MAIN] Parsing finalizado: ACCEPT\n");
    // else
    //     fprintf(ofile, "[MAIN] Parsing finalizado: ERROR\n");

 
    fclose(dbg_out);

    return 0;
}