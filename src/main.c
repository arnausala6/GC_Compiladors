#include "./main.h"
#include "./module_args/module_args.h"
#include "./scanner/scanner_core.h"
#include "./token_model/token_model.h"
#include "./diagnostics/diagnostics.h"
#include "./counters/counters.h"
#include "output_writer/output_writer.h"

FILE* ofile = NULL; 

Scanner s;
TokenList tl;
OutputWriter ow;
Diagnostics diag;
Counters counters;

void generate_output_filename(const char* input, char* output, const char* new_ext) {
    strcpy(output, input);
    char *dot = strrchr(output, '.');
    if (dot) {
        strcpy(dot + 1, new_ext);
    } else {
        strcat(output, ".");
        strcat(output, new_ext);
    }
}

int main(int argc, char *argv[]) {
    // 1. Inicializar log del sistema (stdout o fichero log general)
    ofile = set_output_test_file(PROJOUTFILENAME);

    AppConfig config;
    char *input_filename = NULL;

    // 2. Procesar argumentos de terminal
    if (process_arguments(argc, argv, &config, &input_filename) != 0) {
        return 1;
    }

    fprintf(ofile, "[MAIN] Configuracion cargada. Input: %s, Mode: %d\n", 
            input_filename, config.outformat);

    // 3. Preparar contexto de ficheros (FileCtx)
    FileCtx files;
    files.input_path = input_filename;
    
    // 3.1 Abrir Entrada
    files.input_file = fopen(input_filename, "r");
    if (!files.input_file) {
        perror("Error abriendo fichero de entrada");
        return 1;
    }

    // 3.2 Preparar y Abrir Salida (.cscn) 
    char out_name[256];
    generate_output_filename(input_filename, out_name, "cscn");
    files.output_file = fopen(out_name, "w");
    if (!files.output_file) {
        perror("Error creando fichero de salida .cscn");
        fclose(files.input_file);
        return 1;
    }
    fprintf(ofile, "[MAIN] Fichero salida generado: %s\n", out_name);

    // 3.3 Preparar fichero de conteo (.dbgcnt) si es necesario 
    files.dbgcnt_file = NULL;
    if (config.count_enabled) {
        char cnt_name[256];
        generate_output_filename(input_filename, cnt_name, "dbgcnt");
        files.dbgcnt_file = fopen(cnt_name, "w");
        if (!files.dbgcnt_file) {
            fprintf(stderr, "Warning: No se pudo crear fichero .dbgcnt\n");
        } else {
            fprintf(ofile, "[MAIN] Fichero debug count generado: %s\n", cnt_name);
        }
    }
    
    diagnostics_init(&diag, stdout, files.output_file);
    counters_init(&counters);
    tokenlist_init(&tl);
    output_writer_init(&ow, files.output_file, config.outformat, config.debug_on);
    scanner_init(&s, files.input_file, input_filename, &tl, &diag, &counters);
    scanner_run(&s);
    for(int i=0; i < tl.size; i++) {
        output_writer_write_token(&ow, &tl.data[i]);
    }
    fprintf(ofile, "[MAIN] Scanner finalizado (Simulado).\n");

    // 5. Limpieza
    if (files.input_file) fclose(files.input_file);
    if (files.output_file) fclose(files.output_file);
    if (files.dbgcnt_file) fclose(files.dbgcnt_file);
    output_writer_close(&ow);

    return 0;
}