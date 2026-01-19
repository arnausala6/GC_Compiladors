#ifndef DELETE_COMMENTS_H
#define DELETE_COMMENTS_H

#include <stdio.h>

/*
 * Módulo: delete_comments
 * Responsabilidad: eliminar comentarios de C cuando el motor detecta el inicio.
 * El motor llama a manejar_comentario() justo después de leer '/' y confirmar
 * que el siguiente carácter es '/' o '*', dejando ese segundo carácter en el
 * stream con ungetc().
 */

typedef enum {
    DC_OK = 0,
    DC_ERROR_PARAM = 1
} dc_result_t;

/*
 * Maneja un comentario ya detectado por el motor.
 *
 * Contrato de entrada (IMPORTANTE):
 * - El carácter '/' inicial YA ha sido consumido por el motor.
 * - El siguiente carácter en 'in' es '/' o '*', porque el motor lo ha dejado con ungetc().
 *
 * Efecto:
 * - Escribe un espacio en 'out' sustituyendo el comentario.
 * - Consume el comentario:
 *     - '//' hasta '\n' (escribe también el '\n' para mantener líneas)
 *     - '/ * ... * /' hasta el cierre (mantiene '\n' que aparezcan dentro)
 * - Actualiza *linea_actual al encontrar '\n'
 *
 * Devuelve:
 * - DC_OK si todo correcto
 * - DC_ERROR_PARAM si parámetros inválidos
 */
dc_result_t manejar_comentario(FILE *in, FILE *out, int *linea_actual);

/*
 * (Opcional) Procesa un fichero completo eliminando comentarios.
 * Útil para tests aislados. Internamente reutiliza manejar_comentario().
 */
dc_result_t procesar_fichero_sin_comentarios(FILE *in, FILE *out, int *linea_actual);

#endif
