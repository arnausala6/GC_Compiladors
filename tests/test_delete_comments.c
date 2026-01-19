#include <stdio.h>
#include "../src/delete_comments/delete_comments.h"

int main(void) {
    FILE *in = fopen("src/delete_comments/tests/input_comments.c", "r");
    FILE *out = fopen("src/delete_comments/tests/output_comments.c", "w");

    if (!in) {
        perror("Error abriendo input_comments.c");
        return 1;
    }
    if (!out) {
        perror("Error creando output_comments.c");
        fclose(in);
        return 1;
    }

    int rc = dc_process_file(in, out);

    fclose(in);
    fclose(out);

    if (rc != 0) {
        fprintf(stderr, "dc_process_file devolvió error %d\n", rc);
        return 1;
    }

    printf("delete_comments ejecutado correctamente\n");
    return 0;
}
