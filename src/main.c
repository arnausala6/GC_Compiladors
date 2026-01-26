#include "./main.h"


FILE* ofile = NULL; // The output handler for the project run (same variable name as in modules)

int main(int argc, char *argv[]) {

    ofile = stdout; // Default output to stdout
    ofile = set_output_test_file(PROJOUTFILENAME);

    fprintf(ofile, "Starting module args ...\n");
    int mode = process_arguments(argc, argv);
    if(mode == -1) {
        fprintf(ofile, "Error processing arguments. Exiting.\n");
        fclose(ofile);
        return 1;
    }
    const char *input_path = argv[1];
    printf("Archivo de entrada: %s\n", argv[1]);
    fflush(stdout); // Fuerza a la consola a imprimir lo que tenga en memoria
    FILE *input_file = fopen(argv[1], "r");

    Tabla_macros macros = {0};
    IfStack ifstack;
    GDError err = {0};
    memset(&err, 0, sizeof(err));
    macros.elementos = 0;
    macros.macros = NULL;
    ifsinit(&ifstack); 
    
    int rc = motor_preprocesador(input_file, ofile, mode, input_path, &macros, &ifstack, &err);
    if (rc != 0) fprintf(stderr, "ERR %s:%d:%d %s\n", input_path, 0, 0, "Error in preprocessor");
    fprintf(ofile, "All modules executed successfully!\n\n");

    fclose(ofile); 
    return 0;
}