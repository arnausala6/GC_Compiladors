#include "preprocessor.h"
#include "../module_replacedir/replace_dir.h"
#include "../delete_comments/delete_comments.h"
#include "../macro_substitute/macro_substitute.h"

#define MAX_IDENT 256

/**
 * @brief Maneja y procesa literales de cadena (strings) y caracteres.
 * * Esta función se activa cuando el motor encuentra una comilla simple o doble.
 * Su objetivo es copiar el contenido del literal directamente al archivo de 
 * salida sin procesar macros o comentarios dentro de él, respetando las
 * secuencias de escape (como \") y manteniendo el conteo de líneas.
 * * @param in           Puntero al archivo de entrada.
 * @param out          Puntero al archivo de salida.
 * @param quote        El tipo de comilla detectada (' o ").
 * @param linea_actual Puntero al contador de líneas global para actualizarlo si hay saltos de línea.
 * @param ifstack      Pila de condicionales para verificar si el bloque actual debe escribirse.
 * @return int         0 si finaliza correctamente, 1 si hay error de parámetros.
 */
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

/*
 * @brief Función principal del motor de preprocesamiento.
 * * Implementa una máquina de estados para procesar el código fuente. Se encarga de:
 * 1. Detectar directivas de preprocesador (empezando con # en una línea nueva).
 * 2. Identificar y eliminar comentarios (dependiendo de los flags).
 * 3. Reconocer identificadores y llamar al módulo de sustitución de macros.
 * 4. Gestionar bloques condicionales (#ifdef, #ifndef, etc.) mediante ifstack.
 * @param in        Puntero al archivo fuente de entrada.
 * @param out       Puntero al archivo de destino procesado.
 * @param flags     Modo de operación: 0 (comentarios), 1 (directivas), 2 (ambos).
 * @param fullpath  Ruta del archivo actual (para manejo de errores e inclusiones).
 * @param macros    Tabla de símbolos que contiene las macros definidas.
 * @param ifstack   Estructura de pila para el control de flujo de directivas condicionales.
 * @param err       Estructura para el reporte y gestión de errores.
 * @return int      0 si el proceso es exitoso, 1 si ocurre un error crítico.
 */
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
    char buffer[MAX_IDENT];
    int buf_idx;
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
                        // printf("He entrado aqui\n");
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
                if (
                    (c >= 'a' && c <= 'z') ||
                    (c >= 'A' && c <= 'Z') ||
                    c == '_' ) 
                    {
                    buf_idx = 0;
                    buffer[buf_idx++] = (char)c;

                    while ((siguiente = fgetc(in)) != EOF) {
                        if (
                            (siguiente >= 'a' && siguiente <= 'z') ||
                            (siguiente >= 'A' && siguiente <= 'Z') ||
                            (siguiente >= '0' && siguiente <= '9') ||
                            siguiente == '_'
                        ) {
                            if (buf_idx < MAX_IDENT - 1) {
                                buffer[buf_idx++] = (char)siguiente;
                            }
                        } else {
                            ungetc(siguiente, in);
                            break;
                        }
                }

                buffer[buf_idx] = '\0';

                /* Delegación TOTAL al módulo de sustitución */
                if (ifs_is_active(ifstack)) {
                    if (!sustituir_macro(buffer, macros, out)) {
                        fputs(buffer, out);
                    }
                }

                break;
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
