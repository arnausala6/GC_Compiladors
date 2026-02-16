/*
* -----------------------------------------------------------------------------
* Programa: P2 – Lexer
* Archivo: module_args.c
* Autor: Raúl 
* Fecha: 9/02/2026
*
 * Descripción:
 *  Este archivo implementa el procesador de argumentos de línea de comandos, que es responsable 
 *  de interpretar los argumentos pasados al programa.
 * 
 * El procesador de argumentos incluye:
 * - La definición de la estructura AppConfig, que almacena la configuración del programa. 
 * - Funciones para procesar los argumentos y llenar la estructura AppConfig.
* -----------------------------------------------------------------------------
*/

#include "./module_args.h"

void print_help(const char *prog_name) {
    printf("Uso: %s %s <fichero.c> [opciones]\n", prog_name, FLAG_INPUT);
    printf("Opciones:\n");
    printf("  %s %s|%s   : Define formato salida (Release/Debug)\n", FLAG_MODE, VAL_RELEASE, VAL_DEBUG);
    printf("  %s             : Activa trazas de depuracion (DEBUG=1)\n", FLAG_TRACE);
    printf("  %s             : Activa conteo de operaciones\n", FLAG_COUNT);
}

int process_arguments(int argc, char *argv[], AppConfig *config, char **input_filename) {
    // Valores por defecto
    config->outformat = OUTFORMAT_RELEASE;
    config->debug_on = 0;
    config->count_enabled = 0;
    config->countout = 0; // Por defecto a fichero
    *input_filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], FLAG_INPUT) == 0) {
            if (i + 1 < argc) {
                *input_filename = argv[++i];
            } else {
                fprintf(stderr, "Error: %s requiere un nombre de archivo.\n", FLAG_INPUT);
                return 1;
            }
        } 
        else if (strcmp(argv[i], FLAG_MODE) == 0) {
            if (i + 1 < argc) {
                char *val = argv[++i];
                if (strcmp(val, VAL_DEBUG) == 0) {
                    config->outformat = OUTFORMAT_DEBUG;
                } else {
                    config->outformat = OUTFORMAT_RELEASE;
                }
            }
        }
        else if (strcmp(argv[i], FLAG_TRACE) == 0) {
            config->debug_on = 1;
        }
        else if (strcmp(argv[i], FLAG_COUNT) == 0) {
            config->count_enabled = 1;
        }
    }

    if (*input_filename == NULL) {
        fprintf(stderr, "Error: No se ha especificado archivo de entrada.\n");
        print_help(argv[0]);
        return 1;
    }

    return 0;
}

void print_arguments(int argc, char *argv[]) {
    fprintf(ofile, "Arguments received (%d):\n", argc);
    for (int i = 0; i < argc; i++) {
        fprintf(ofile, "Argument %d: %s\n", i, argv[i]);
    }
    fflush(ofile);
}

