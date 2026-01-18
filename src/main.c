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
    FILE *input_file = fopen(argv[1], "r");
    motor_preprocesador(input_file, ofile, mode);
    fprintf(ofile, "All modules executed successfully!\n\n");

    fclose(ofile); 
    return 0;
}