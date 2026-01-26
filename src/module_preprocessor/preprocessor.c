#include "preprocessor.h"
#include "../module_replacedir/replace_dir.h"
#include "../delete_comments/delete_comments.h"

static int manejar_strings(
    FILE *in,
    FILE *out,
    int quote,
    int *linea_actual,
    IfStack *ifstack
) {
    if (!in || !out || !linea_actual || !ifstack) return 1;

    if (ifs_is_active(ifstack)) fputc(quote, out);

    int c;
    while ((c = fgetc(in)) != EOF) {

        if (ifs_is_active(ifstack)) fputc(c, out);

        if (c == '\n') (*linea_actual)++;

        if (c == '\\') {
            int next = fgetc(in);
            if (next == EOF) return 0;
            if (ifs_is_active(ifstack)) fputc(next, out);
            if (next == '\n') (*linea_actual)++;
            continue;
        }

        if (c == quote) return 0;
    }

    return 0;
}

int motor_preprocesador(
    FILE *in, FILE *out, int flags,
    const char *fullpath,
    Tabla_macros *macros,
    IfStack *ifstack,
    GDError *err
) {
    int c;
    int siguiente;
    int linea_actual = 1;
    EstadoMotor estado = ESTADO_LINEA_NUEVA;

    if (!in || !out || !fullpath || !macros || !ifstack || !err) return 1;

    while ((c = fgetc(in)) != EOF) {

        switch (estado) {

            case ESTADO_LINEA_NUEVA:
                if (c == ' ' || c == '\t') {
                    if (ifs_is_active(ifstack)) fputc(c, out);
                    break;
                }

                if (c == '#') {
                    if (flags == 1 || flags == 2) {
                        printf("He entrado aqui\n");
                        int rc = replace_directives_handle_hash(
                            in, out, &linea_actual, flags,
                            macros, err, ifstack, fullpath
                        );
                        if (rc != 0) return 1;
                    } else {
                        if (ifs_is_active(ifstack)) fputc(c, out);
                    }

                    // IMPORTANTE: después de una directiva ya estás en nueva línea
                    estado = ESTADO_LINEA_NUEVA;
                    break;
                }

                ungetc(c, in);
                estado = ESTADO_NORMAL;
                break;

            case ESTADO_NORMAL:
                c = fgetc(in);
                if (c == EOF) return 0;

                if (c == '"' || c == '\'') {
                    // NO imprimir aquí; lo hace manejar_strings 
                    manejar_strings(in, out, c, &linea_actual, ifstack);
                    break;
                }

                if (c == '/') {
                    siguiente = fgetc(in);
                    int es_comentario = (siguiente == '/' || siguiente == '*');

                    if (es_comentario && (flags == 0 || flags == 2)) {
                        ungetc(siguiente, in);
                        manejar_comentario(in, out, &linea_actual);
                        break;
                    } else {
                        if (ifs_is_active(ifstack)) fputc(c, out);
                        if (siguiente != EOF) ungetc(siguiente, in);
                        break; 
                    }
                }

                if (c == '\n') {
                    if (ifs_is_active(ifstack)) fputc(c, out);
                    linea_actual++;
                    estado = ESTADO_LINEA_NUEVA;
                    break;
                }

                if (ifs_is_active(ifstack)) fputc(c, out);
                break;

            default:
                estado = ESTADO_NORMAL;
                break;
        }
    }

    return 0;
}
