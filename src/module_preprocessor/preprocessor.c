#include "preprocessor.h"
#include "./../delete_comments/delete_comments.h"

/*
 * MOTOR PRINCIPAL DEL PREPROCESADOR
 * * Flags:
 * 0 (-c)   : Elimina comentarios, ignora directivas. [cite: 22]
 * 1 (-d)   : Mantiene comentarios, procesa directivas. [cite: 23]
 * 2 (-all) : Elimina comentarios, procesa directivas. [cite: 24]
 */
void motor_preprocesador(FILE *in, FILE *out, int flags) {
    int c;
    int siguiente;
    int linea_actual = 1;
    EstadoMotor estado = ESTADO_LINEA_NUEVA;

    // TODO: Inicializar la pila de IfStack para ifdef anidados
    // TODO: Inicializar Tabla de macros

    while ((c = fgetc(in)) != EOF) {
        
        switch (estado) {
            
            // --- ESTADO: COMIENZO DE LÍNEA ---
            // Buscamos '#' para directivas
            case ESTADO_LINEA_NUEVA:
                // Ignorar espacios en blanco al inicio de línea
                if (c == ' ' || c == '\t') {
                    fputc(c, out);
                    break;
                }
                
                // Si encontramos '#', verificamos si debemos procesar directivas
                if (c == '#') {
                    // Solo procesamos directivas en modo -d (1) o -all (2)
                    if (flags == 1 || flags == 2) {
                        // AQUÍ IRÁ LA LÓGICA DE DIRECTIVAS
                        // Por ahora, solo lo imprimimos para depurar
                         fprintf(out, "#"); 
                         // parsear_directiva(...);
                    } else {
                        // En modo -c, el '#' es texto normal
                        fputc(c, out);
                    }
                    // Tras un #, volvemos a estado normal hasta el salto de línea
                    estado = ESTADO_NORMAL;
                } else {
                    // No es directiva, retrocedemos y cambiamos a estado normal
                    ungetc(c, in);
                    estado = ESTADO_NORMAL;
                }
                break;

            // --- ESTADO: TEXTO NORMAL ---
            case ESTADO_NORMAL:
                // 1. Detección de Strings (para no borrar comentarios dentro de strings)
                if (c == '"' || c == '\'') {
                    fputc(c, out);
                    // Aquí deberías consumir el string completo para evitar 
                    // falsos positivos de comentarios dentro de comillas.
                    // manejar_string(in, out, c, &linea_actual); 
                }
                
                // 2. Detección de Comentarios
                else if (c == '/') {
                    siguiente = fgetc(in);
                    
                    int es_comentario = (siguiente == '/' || siguiente == '*');
                    
                    // Verificamos si debemos eliminar comentarios (flags 0 o 2) [cite: 19]
                    if (es_comentario && (flags == 0 || flags == 2)) {
                        // Devolvemos el carácter 'siguiente' al buffer porque 
                        // manejar_comentario espera leer el tipo ('/' o '*')
                        ungetc(siguiente, in); 
                        manejar_comentario(in, out, &linea_actual);
                    } 
                    else {
                        // O no es comentario, o estamos en modo -d (conservar comentarios)
                        fputc(c, out);
                        if (siguiente != EOF) {
                            ungetc(siguiente, in);
                        }
                    }
                }
                
                // 3. Gestión de saltos de línea (resetea el estado)
                else {
                    fputc(c, out);
                    if (c == '\n') {
                        linea_actual++;
                        estado = ESTADO_LINEA_NUEVA;
                    }
                }
                break;

            default:
                fputc(c, out);
                estado = ESTADO_NORMAL;
                break;
        }
    }
}