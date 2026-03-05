#include "language.h"

// --- Funciones de búsqueda en el diccionario ---

int language_find_terminal(Language *lang, const char *name) {
    for (int i = 0; i < lang->num_terminals; i++) {
        if (strcmp(lang->terminals[i].name, name) == 0) {
            return lang->terminals[i].id;
        }
    }
    return -1;
}

int language_find_nonterminal(Language *lang, const char *name) {
    for (int i = 0; i < lang->num_nonterminals; i++) {
        if (strcmp(lang->nonterminals[i].name, name) == 0) {
            return lang->nonterminals[i].id;
        }
    }
    return -1;
}

// --- Función principal de carga ---

int language_load(Language *lang, const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return 1; // Error abriendo el archivo
    }

    // Inicializar el lenguaje a cero/valores por defecto
    lang->num_terminals = 0;
    lang->num_nonterminals = 0;
    lang->num_prods = 0;
    lang->dfa_tables.num_states = 0;
    lang->start_symbol = 0; 

    // Inicializar matrices de autómata a estado de ERROR y Goto a -1
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_TERMINALS; j++) {
            lang->dfa_tables.action[i][j].type = ACT_ERROR;
            lang->dfa_tables.action[i][j].value = 0;
        }
        for (int j = 0; j < MAX_NONTERMINALS; j++) {
            lang->dfa_tables.goto_table[i][j] = -1;
        }
    }

    char line[256];
    ParseMode mode = MODE_NONE;
    int max_state_found = -1; // Para el cálculo dinámico de num_states

    while (fgets(line, sizeof(line), file)) {
        // Ignorar comentarios (//) y saltos de línea vacíos
        if (strncmp(line, "//", 2) == 0 || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        // Cambios de sección (Bloques del txt)
        if (strncmp(line, "%TERMINALS", 10) == 0) { mode = MODE_TERM; continue; }
        if (strncmp(line, "%NONTERMINALS", 13) == 0) { mode = MODE_NONTERM; continue; }
        if (strncmp(line, "%PRODUCTIONS", 12) == 0) { mode = MODE_PROD; continue; }
        if (strncmp(line, "%ACTION_TABLE", 13) == 0) { mode = MODE_ACTION; continue; }
        if (strncmp(line, "%GOTO_TABLE", 11) == 0) { mode = MODE_GOTO; continue; }

        // Procesar según el bloque actual
        if (mode == MODE_TERM || mode == MODE_NONTERM) {
            // Lee todos los símbolos separados por espacio en la línea
            char *token = strtok(line, " \t\r\n");
            while (token) {
                if (mode == MODE_TERM) {
                    strcpy(lang->terminals[lang->num_terminals].name, token);
                    lang->terminals[lang->num_terminals].id = lang->num_terminals;
                    lang->num_terminals++;
                } else {
                    strcpy(lang->nonterminals[lang->num_nonterminals].name, token);
                    lang->nonterminals[lang->num_nonterminals].id = lang->num_nonterminals;
                    lang->num_nonterminals++;
                }
                token = strtok(NULL, " \t\r\n");
            }
        } 
        else if (mode == MODE_PROD) {
            // Formato: "0: s -> e" o "1: e -> e + t"
            int prod_id;
            char lhs_str[MAX_LENGTH];
            sscanf(line, "%d: %s ->", &prod_id, lhs_str);
            
            lang->prods[prod_id].lhs = language_find_nonterminal(lang, lhs_str);
            
            // Lógica sencilla para contar RHS (contar tokens tras "->")
            char *arrow = strstr(line, "->");
            if (arrow) {
                arrow += 2;
                int count = 0;
                char *rhs_tok = strtok(arrow, " \t\r\n");
                while(rhs_tok) {
                    count++;
                    rhs_tok = strtok(NULL, " \t\r\n");
                }
                lang->prods[prod_id].rhs_len = count;
            }
            lang->num_prods++;
        }
        else if (mode == MODE_ACTION) {
            // Formato: 2 + REDUCE 2
            int state, value = 0;
            char term_str[MAX_LENGTH], act_str[MAX_LENGTH];
            
            int parsed = sscanf(line, "%d %s %s %d", &state, term_str, act_str, &value);
            if (parsed >= 3) {
                int term_id = language_find_terminal(lang, term_str);
                if (term_id != -1) {
                    ActionEntry *entry = &lang->dfa_tables.action[state][term_id];
                    if (strcmp(act_str, "SHIFT") == 0) entry->type = ACT_SHIFT;
                    else if (strcmp(act_str, "REDUCE") == 0) entry->type = ACT_REDUCE;
                    else if (strcmp(act_str, "ACCEPT") == 0) entry->type = ACT_ACCEPT;
                    
                    entry->value = value;
                }
                if (state > max_state_found) max_state_found = state;
            }
        }
        else if (mode == MODE_GOTO) {
            // Formato: 0 e 1
            int state, next_state;
            char nonterm_str[MAX_LENGTH];
            if (sscanf(line, "%d %s %d", &state, nonterm_str, &next_state) == 3) {
                int nonterm_id = language_find_nonterminal(lang, nonterm_str);
                if (nonterm_id != -1) {
                    lang->dfa_tables.goto_table[state][nonterm_id] = next_state;
                }
                if (state > max_state_found) max_state_found = state;
                if (next_state > max_state_found) max_state_found = next_state;
            }
        }
    }

    // Asignación de la Opción 2: Cálculo Dinámico de Estados
    lang->dfa_tables.num_states = max_state_found + 1;

    fclose(file);
    return 0; // Éxito
}