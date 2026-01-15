/*
delete_comments.c
---------------------------------------------------------------------------

Implementación de la lógica de eliminación de comentarios para el pre-procesador P1PP.

Funcionalidad:
    - Elimina comentarios de una línea: //
    - Elimina comentarios de bloque:   / * ... * /
    - Sustituye cada comentario eliminado por un espacio.
    - Mantiene los saltos de línea fuera de los comentarios para que no se colapsen líneas de código distintas.

  Notas:
      - Este módulo no conoce nada de directivas ni macros, sólo de texto.
      - Diseñado como motor independiente para poder probarlo con un main
        de test o integrarlo en el preprocesador general.
---------------------------------------------------------------------------
 */

#include "delete_comments.h"

/* Función interna para escribir un carácter si no es EOF */
static void dc_putc(int c, FILE *out)
{
    if (c != EOF) {
        fputc(c, out);
    }
}

void dc_init_context(dc_context_t *ctx)
{
    if (!ctx) return;
    ctx->state = DC_STATE_NORMAL;
    ctx->has_emitted_space_for_comment = 0;
}

/*
Lógica principal:
    - Recorre el fichero de entrada carácter a carácter.
    - Aplica la máquina de estados para reconocer comentarios.
    - Escribe en out el texto limpio de comentarios.
*/
int dc_process_file(FILE *in, FILE *out)
{
    int c;
    dc_context_t ctx;

    if (!in || !out) {
        return 1; // Error simple; se puede refinar más adelante
    }

    dc_init_context(&ctx);

    while ((c = fgetc(in)) != EOF) {

        switch (ctx.state) {
        case DC_STATE_NORMAL:
            if (c == '/') {
                // Podría empezar comentario, esperamos al siguiente char
                ctx.state = DC_STATE_SLASH;
            } else {
                dc_putc(c, out);
            }
            break;

        case DC_STATE_SLASH:
            if (c == '/') {
                // Inicio de comentario de línea "//"
                if (!ctx.has_emitted_space_for_comment) {
                    dc_putc(' ', out);
                    ctx.has_emitted_space_for_comment = 1;
                }
                ctx.state = DC_STATE_LINE_COMMENT;
            } else if (c == '*') {
                // Inicio de comentario de bloque "/*"
                if (!ctx.has_emitted_space_for_comment) {
                    dc_putc(' ', out);
                    ctx.has_emitted_space_for_comment = 1;
                }
                ctx.state = DC_STATE_BLOCK_COMMENT;
            } else {
                // No era comentario, el '/' era código normal
                dc_putc('/', out);
                dc_putc(c, out);
                ctx.state = DC_STATE_NORMAL;
            }
            break;

        case DC_STATE_LINE_COMMENT:
            if (c == '\n') {
                // Fin de comentario de línea: mantenemos el salto de línea
                dc_putc('\n', out);
                ctx.state = DC_STATE_NORMAL;
                ctx.has_emitted_space_for_comment = 0;
            }
            // Si no es '\n', simplemente lo ignoramos
            break;

        case DC_STATE_BLOCK_COMMENT:
            if (c == '*') {
                ctx.state = DC_STATE_BLOCK_COMMENT_STAR;
            }
            // Cualquier otro carácter dentro del bloque se ignora
            break;

        case DC_STATE_BLOCK_COMMENT_STAR:
            if (c == '/') {
                // Cerramos el bloque "*/"
                ctx.state = DC_STATE_NORMAL;
                ctx.has_emitted_space_for_comment = 0;
            } else if (c == '*') {
                // Seguimos en posible cierre
                ctx.state = DC_STATE_BLOCK_COMMENT_STAR;
            } else {
                // Seguimos dentro del comentario de bloque
                ctx.state = DC_STATE_BLOCK_COMMENT;
            }
            break;

        default:
            // Estado inesperado: reiniciamos a algo seguro
            ctx.state = DC_STATE_NORMAL;
            break;
        }
    }

    // Si el fichero termina mientras estamos en: - DC_STATE_SLASH: significa que vimos un '/' 
    // al final sin nada después. En ese caso, lo emitimos como código normal.

    if (ctx.state == DC_STATE_SLASH) {
        dc_putc('/', out);
    }

    // No distinguimos de momento errores de sintaxis (bloque sin cerrar, etc.)
    return 0;
}