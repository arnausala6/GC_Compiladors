#include "user_manual.h"
#include <string.h>

#define MANPAGE_PATH "user_manual.txt"

int print_help(FILE *out) {
    FILE *fp;
    char buffer[512];

    if (!out) return 1;

    fp = fopen(MANPAGE_PATH, "r");
    if (!fp) {
        fprintf(out, "Error loading the user manual.\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fputs(buffer, out);
    }

    fclose(fp);
    return 0;
}