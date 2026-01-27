#include "./main.h"
#include "./module_preprocessor/preprocessor.h"
#include "./module_replacedir/replace_dir.h"
#include "./macrostoring/macrostoring.h"

void generar_nombre_pp(const char *entrada, char *salida) {
    strcpy(salida, entrada);
    char *punto = strrchr(salida, '.');
    if (punto) {
        // Guardamos la extensión original (ej: .c)
        char extension[10];
        strcpy(extension, punto);
        // Insertamos _pp antes de la extensión
        sprintf(punto, "_pp%s", extension);
    } else {
        // Si no tiene extensión, añadimos _pp.c
        strcat(salida, "_pp.c");
    }
}

FILE* ofile = NULL; 

int main(int argc, char *argv[]) {

    ofile = stdout; 
    ofile = set_output_test_file(PROJOUTFILENAME); // ofile se usa para LOGS del sistema

    if (argc < 2) {
        fprintf(ofile, "Error: Missing input file argument.\n");
        printf("Usage: %s <filename.c>\n", argv[0]);
        return 1;
    }

    fprintf(ofile, "Starting module args ...\n");
    int mode = process_arguments(argc, argv);
    
    if(mode == -1) {
        fprintf(ofile, "Error processing arguments. Exiting.\n");
        fclose(ofile);
        return 1;
    }

    if (mode == 3)
    {
        print_help(ofile);
        fclose(ofile);
        return 0;
    }
    

    // 1. Abrir archivo de ENTRADA
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(ofile, "Error opening input file: %s\n", argv[1]);
        perror("Error");
        return 1;
    }

    // 2. Generar nombre y Abrir archivo de SALIDA (_pp.c)
    char pp_filename[256];
    const char *input_path = argv[1];
    generar_nombre_pp(argv[1], pp_filename);
    
    FILE *pp_file = fopen(pp_filename, "w");
    if (!pp_file) {
        fprintf(ofile, "Error creating output file: %s\n", pp_filename);
        fclose(input_file);
        return 1;
    }

    // Informamos al usuario (por consola)
    printf("Procesando: %s -> %s\n", argv[1], pp_filename);
    fprintf(ofile, "Generated output file: %s\n", pp_filename);

    // 3. Iniciamos la pila, la tabla de macros y el GDError y ejecutamos el motor (escribiendo en pp_file, no en ofile)
    Tabla_macros macros = {0};
    IfStack ifstack;
    GDError err = {0};
    memset(&err, 0, sizeof(err));
    macros.elementos = 0;
    macros.macros = NULL;
    ifs_init(&ifstack); 


    
    int rc = motor_preprocesador(input_file, pp_file, mode, input_path, &macros, &ifstack, &err);
    if (rc != 0) fprintf(stderr, "ERR %s:%d:%d %s\n", input_path, 0, 0, "Error in preprocessor");
    fprintf(ofile, "All modules executed successfully!\n\n");

    // Cerrar todo
    fclose(input_file);
    fclose(pp_file);
    if (ofile != stdout) fclose(ofile); 
    
    return 0;
}
