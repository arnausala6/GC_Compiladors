/*
 * -----------------------------------------------------------------------------
 * module_args.c 
 *
 * This module provides functionality to process command-line arguments.
 * 
 * - `print_arguments`: Prints each command-line argument with its index.
 * - `process_arguments`: Intended to handle application-specific argument logic.
 *                        Currently, it calls `print_arguments` and signals that
 *                        it's not implemented yet.
 *
 * Usage:
 *     Called from the main application or test modules to process CLI args.
 *
 * Status:
 *     Initial development — logic to handle arguments is not yet implemented.
 *
 * Author: [Your Name or Team]
 * -----------------------------------------------------------------------------
 */

#include "./module_args.h"

void print_arguments(int argc, char *argv[]) {
    fprintf(ofile, "Arguments received (%d):\n", argc);
    for (int i = 0; i < argc; i++) {
        fprintf(ofile, "Argument %d: %s\n", i, argv[i]);
    }
    fflush(ofile);
}

int process_arguments(int argc, char *argv[]) {
    print_arguments(argc, argv);
    int mode = 0; // modo por defecto (-c mode)
    
    if (argc >= 2 && strcmp(argv[1], "-help") == 0) {
        return 3;
    }

    for(int i = 2; i < argc; i++) {
        if(strcmp(argv[i], "-c") == 0) {
            mode = 0;
            if(strcmp(argv[i-1], "-d") == 0){
                mode = 2; // si -d viene antes de -c, activar modo -all
                return mode;
            }
        }

        else if(strcmp(argv[i], "-d") == 0) {
            mode = 1;
            if(strcmp(argv[i-1], "-c") == 0){
                mode = 2; // si -c viene antes de -d, activar modo -all
                return mode;
            }
        }

        else if(strcmp(argv[i], "-all") == 0) {
            mode = 2;
            return mode;
        }

        else if(strcmp(argv[i], "-help") == 0) {
            mode = 3;
            return mode;
        }

        else {
            fprintf(ofile, "Argument %s not recognized. Exiting.\n", argv[i]);
            return -1;
        }
    }
    fflush(ofile);
    return mode;
}
