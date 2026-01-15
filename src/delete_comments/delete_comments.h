#ifndef DELETE_COMMENTS_H
#define DELETE_COMMENTS_H

#include <stdio.h>

/*
 * ---------------------------------------------------------------------------
 *  delete_comments.h
 *
 *  Funcionalidad:
 *      Proporciona las funciones para eliminar comentarios de código C
 *      (// y / * * /) sobre un flujo de entrada y escribir el resultado
 *      limpio en un flujo de salida.
 *
 *      Esta fase está pensada como un componente del pre-procesador P1PP,
 *      pero puede probarse de forma independiente.
 *
 *  Alcance:
 *      - Elimina comentarios de una línea: //
 *      - Elimina comentarios de bloque:   / * ... * /
 *      - Sustituye cada comentario por al menos un espacio.
 *      - No elimina líneas vacías explícitas del fichero original.
 *
 *  Autores:
 *      Arnau Sala  (delete_comments)
 *      <añadir resto si lo acordáis>
 * ---------------------------------------------------------------------------
 */

/* 
 * Estado interno del eliminador de comentarios.
 * Se usa para poder procesar el fichero carácter a carácter.
 */

typedef enum {
    DC_STATE_NORMAL = 0,            // Código normal */
    DC_STATE_SLASH,                 // Se ha leído '/' y aún no sabemos si es comentario
    DC_STATE_LINE_COMMENT,          // Comentario // hasta fin de línea
    DC_STATE_BLOCK_COMMENT,         // Dentro de /* ... */
    DC_STATE_BLOCK_COMMENT_STAR     // Hemos visto '*' dentro de bloque, posible cierre
} dc_state_t;


// Estructura de contexto para la eliminación de comentarios. 
// Permite extender el comportamiento en el futuro sin romper la API.

typedef struct {
    dc_state_t state;
    int has_emitted_space_for_comment; /* Para no duplicar espacios al eliminar comentarios */
} dc_context_t;

// Inicializa el contexto de eliminación de comentarios.
// Debe llamarse antes de procesar un fichero.

void dc_init_context(dc_context_t *ctx);

/*
Procesa un fichero completo:
 - Lee desde 'in'
 - Elimina comentarios según el handout
 - Escribe el resultado en 'out'
 
Devuelve:
    0 si se procesa sin errores (desde el punto de vista de este módulo)
    !=0 si se detecta alguna condición de error interna (por definir/extender)
*/

int dc_process_file(FILE *in, FILE *out);

#endif /* DELETE_COMMENTS_H */
