#include "preprocessor.h"
#include "./../delete_comments/delete_comments.h"

/**
 * MOTOR PRINCIPAL DEL PREPROCESADOR
 * Analiza el archivo carácter por carácter y gestiona los estados.
 */
void motor_preprocesador(FILE *in, FILE *out, int flags) {
    int c;
    int siguiente;
    int linea_actual = 1;
    EstadoMotor estado = ESTADO_LINEA_NUEVA;
    // falta incializar la pila de IfStack que sirve para manejar los ifdef anidados
    //IfStack pila;
    //Ifs_init(*pila); del #include "replace_dir.h"
    //Inicializar Tabla de macros
    // Bucle de lectura carácter a carácter hasta el final del archivo [cite: 7, 99]
    while ((c = fgetc(in)) != EOF) {
        
        switch (estado) {
            
            case ESTADO_LINEA_NUEVA:
                // Ignorar espacios en blanco al inicio de línea para detectar la directiva 
                if (c == ' ' || c == '\t') {
                    fputc(c, out);
                    break;
                }
                
                // Si el primer carácter no blanco es '#', procesamos directiva [cite: 8, 9]
                if (c == '#') {
                    printf("Directiva??\n");
                    // puedes usar el guardar_directivas para parsear la directiva y tener su tipo y luego llamas a sustituir directivas
                    // parametros para el replace_dir necesito que pases, la tabla de macros, el ifStack, el nombre del archivo para hacer la recursividad de los include, las flags, y ya lo de la linea actual o la directiva ya depende de como quieras hacaerlo para pasarlo al modulo de sustituir directivas
                    // Tras la directiva, asumimos que volvemos a esperar una nueva línea
                    estado = ESTADO_LINEA_NUEVA;
                } else {
                    // Si no es '#', retrocedemos y procesamos como texto normal
                    ungetc(c, in);
                    estado = ESTADO_NORMAL;
                }
                break;

            case ESTADO_NORMAL:
                // Detección de Strings o Caracteres Literales
                if (c == '"' || c == '\'') {
                    printf("String o Char Literal??\n");
                    //manejar_string(in, out, c, &linea_actual);
                }
                
                // Detección de posibles Comentarios [cite: 197]
                else if (c == '/') {
                    siguiente = fgetc(in);
                    if (siguiente == '/' || siguiente == '*') {
                        ungetc(siguiente, in); // Devolvemos para que la función vea el tipo
                        printf("Comentario\n");
                        manejar_comentario(in, out, &linea_actual);
                    } else {
                        fputc(c, out);
                        if (siguiente != EOF) ungetc(siguiente, in);
                    }
                }
                
                // Detección de posibles Macros (empieza por letra o _) [cite: 141]
                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
                    printf("Macro??\n");
                    //sustituir_macros(char* posible_macro, Tabla_macros, out); --> pasarle a la función una palabra para que la evalue como posible macro
                }
                
                // Gestión de saltos de línea y cambio de estado
                else {
                    fputc(c, out);
                    if (c == '\n') {
                        linea_actual++;
                        estado = ESTADO_LINEA_NUEVA;
                    }
                }
                break;

            default:
                // Por seguridad, si el estado se corrompe, volvemos al normal
                fputc(c, out);
                estado = ESTADO_NORMAL;
                break;
        }
    }
}
