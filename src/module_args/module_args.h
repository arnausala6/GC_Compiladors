#ifndef MODULE2_H
#define MODULE2_H

#include "../main.h"

void print_arguments(int argc, char *argv[]);
int process_arguments(int argc, char *argv[], AppConfig *config, char **input_filename);
void print_help(const char *prog_name);

#endif

