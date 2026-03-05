#ifndef DEBUG_H
#define DEBUG_H

/*
 * Programa: P3 – Bottom-up Parsing (Shift-Reduce Automaton)
 * Archivo:  debug.h
 *
 * Descripción:
 *   Interfaz pública del módulo de traza de depuración.  Formatea y escribe
 *   una línea por paso del autómata SRA en un fichero de traza.  El módulo
 *   es "write-only": no decide acciones de parsing ni modifica la pila ni
 *   el flujo de tokens.
 */

#include <stdio.h>
#include "../module_stack/stack.h"

/* ------------------------------------------------------------------ */
/*  Nombres de operación                                               */
/* ------------------------------------------------------------------ */
#define DBG_OP_SHIFT    "SHIFT"
#define DBG_OP_REDUCE   "REDUCE"
#define DBG_OP_ACCEPT   "ACCEPT"
#define DBG_OP_ERROR    "ERROR"

/* Valor centinela para estado no aplicable (e.g. ACCEPT, ERROR) */
#define DBG_STATE_NA    (-1)

/* ------------------------------------------------------------------ */
/*  Estructura de contexto de traza                                    */
/* ------------------------------------------------------------------ */

typedef struct {
    FILE *out;
    int   step;
} Dbg;

/* ------------------------------------------------------------------ */
/*  API pública                                                        */
/* ------------------------------------------------------------------ */

/*
 * dbg_init   – Inicializa el contexto de traza y escribe la línea de cabecera.
 *              Devuelve 0 en éxito, distinto de 0 si dbg es NULL.
 */
int  dbg_init(Dbg *dbg, FILE *out);

/*
 * dbg_close  – Hace flush del stream.  No cierra el FILE* (eso lo hace main).
 */
void dbg_close(Dbg *dbg);

/*
 * dbg_print_step – Escribe una línea de traza con el estado actual del motor.
 *
 *   dbg         – Contexto de traza (incrementa step internamente).
 *   op          – Nombre de la operación (usar DBG_OP_*).
 *   pos         – Posición actual en el flujo de tokens.
 *   la_lexeme   – Lexema del lookahead (NULL si no aplicable).
 *   la_category – Nombre de la categoría del lookahead (NULL si no aplicable).
 *   s_prev      – Estado previo del autómata.
 *   s_new       – Estado nuevo (DBG_STATE_NA si no aplicable).
 *   st          – Puntero a la pila (puede ser NULL; se imprime vacía).
 */
void dbg_print_step(Dbg *dbg,
                    const char *op,
                    int pos,
                    const char *la_lexeme,
                    const char *la_category,
                    int s_prev,
                    int s_new,
                    const Stack *st);

#endif /* DEBUG_H */
