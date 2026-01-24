#include "./main.h"

int main(int argc, char *argv[]) {

    FILE *ofile = fopen(PROJOUTFILENAME, "w");
    FILE *infile = fopen(argv[1], "r");
    if(ofile == NULL) {
        ofile = stdout;
    }

    fprintf(ofile, "Starting module args ...\n");
    int mode = process_arguments(argc, argv);
    if(mode == -1) {
        printf("Error processing arguments. Exiting.\n");
        fclose(ofile);
        return 1;
    }

    motor_preprocesador(infile, ofile, mode);
    printf("All modules executed successfully!\n\n");

    fclose(ofile); 
    return 0;
}