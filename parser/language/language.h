#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sra_engine/sra_engine.h"
/* --- CONSTANTES DE DIMENSIÓN --- */
#define MAX_LENGTH 32
#define MAX_TERMINALS 64
#define MAX_NONTERMINALS 64
#define MAX_PRODS 64
#define MAX_RHS_LEN 16
#define MAX_STATES 128

/* --- ENUMS Y STRUCTS BASE --- */
/* ActionType y ActionEntry están definidos en automata.h (incluido vía sra_engine.h) */

typedef enum {
    MODE_NONE,
    MODE_TERM,
    MODE_NONTERM,
    MODE_PROD,
    MODE_ACTION,
    MODE_GOTO
} ParseMode;

typedef struct {
    int num_states;
    ActionEntry action[MAX_STATES][MAX_TERMINALS];
    int goto_table[MAX_STATES][MAX_NONTERMINALS];
} AutomatonTables;

typedef struct {
    char name[MAX_LENGTH];
    int id;
} SymbolDef;

typedef struct {
    int lhs;                  // ID del No Terminal (Left-Hand Side)
    int rhs_len;              // Cantidad de símbolos a reducir (para los Pops)
    int rhs[MAX_RHS_LEN];     // Opcional: IDs de los símbolos de la regla (útil para AST futuro)
} Production;

typedef struct Language {
    int start_symbol;
    int num_terminals;
    int num_nonterminals;
    
    SymbolDef terminals[MAX_TERMINALS];
    SymbolDef nonterminals[MAX_NONTERMINALS];
    
    Production prods[MAX_PRODS];
    int num_prods;
    
    AutomatonTables dfa_tables;
    SraEngine engine; 
} Language;

/* --- INTERFAZ DEL MÓDULO --- */

/**
 * Carga la especificación del lenguaje desde un archivo language.txt.
 * @param lang Puntero a la estructura Language a inicializar.
 * @param path Ruta del archivo de texto.
 * @return 0 si fue exitoso, 1 si hubo error.
 */
int language_load(Language *lang, const char *path);

/* Funciones auxiliares expuestas por si otros módulos las necesitan */
int language_find_terminal(Language *lang, const char *name);
int language_find_nonterminal(Language *lang, const char *name);

#endif // LANGUAGE_H