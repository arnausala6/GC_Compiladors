/*
* -----------------------------------------------------------------------------
* Programa: P1PP – Preprocesador
* Archivo: delete_comments.h
* Autor: Arnau Sala
* Fecha: 16/01/2026
*
* Descripción:
* Archivo de cabecera del módulo delete_comments.
* Declara las funciones utilizadas para eliminar comentarios del código 
* fuente y para realizar pruebas del módulo de forma aislada.
*
* Define el contrato entre el motor del preprocesador y el módulo de
* eliminación de comentarios.
* -----------------------------------------------------------------------------
*/



#ifndef DELETE_COMMENTS_H
#define DELETE_COMMENTS_H

#include <stdio.h>

/*
Módulo: delete_comments
Contrato con el motor:
- El motor ya ha leído el primer '/'.
- El siguiente carácter en 'in' es '/' o '*', porque el motor lo dejó con ungetc().
- Esta función sustituye el comentario por un espacio y consume el comentario completo.
- Mantiene '\n' y actualiza *linea_actual.
*/

void manejar_comentario(FILE *in, FILE *out, int *linea_actual);

/*
Función auxiliar para test aislado:
elimina comentarios de todo el fichero (sin depender del motor).
Devuelve 0 si OK, 1 si parámetros inválidos.
 */

int procesar_fichero_sin_comentarios(FILE *in, FILE *out, int *linea_actual);

#endif
