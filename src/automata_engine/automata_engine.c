#include "automata_engine.h"

DFA automatas[NUM_AUTOMATAS];
bool alive[NUM_AUTOMATAS];

void automata_engine_reset(){
    for(int i=0; i<NUM_AUTOMATAS; i++){
        alive[i] = true;
    }
    static DFA init[NUM_AUTOMATAS] = {
    //CAT_NUMBER
    {
        .start_state = 0,
        .states = {0,1},
        .current_state = 0,
        .accept_states = {1},
        .alphabet = {'0','1','2','3','4','5','6','7','8','9'},
    },

    //CAT_IDENTIFIER
    {
        .start_state = 0,
        .states = {0,1,2},
        .current_state = 0,
        .accept_states = {1},
        .alphabet = {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
            '0','1','2','3','4','5','6','7','8','9','_'
        },
    },

    //CAT_KEYWORD
    {
        .start_state = 0,
        .states = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28},
        .current_state = 0,
        .accept_states = {2,4,8,12,16,21,27},
        .alphabet = {'i','f','e','l','s','c','h','a','r','v','o','d','t','u','n','w'},
    },

    //CAT_LITERAL
    {
        .start_state = 0,
        .states = {0,1,2,3},
        .current_state = 0,
        .accept_states = {2},
        .alphabet = {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
            '"'
        },
    },

    //CAT_OPERATOR
    {
        .start_state = 0,
        .states  = {0,1,2},
        .current_state = 0,
        .accept_states = {1},
        .alphabet = {'=','>','+','*'},
    },

    //CAT_SPECIALCHAR
    {
        .start_state = 0,
        .states = {0,1,2,3,4,5,6,7,8,9},
        .current_state = 0,
        .accept_states = {2,4,6,7,8},
        .alphabet = {'(',')','{','}','[',']',';',','},
    }
    };
    for (int i = 0; i < 6; i++) automatas[i] = init[i];

    //Inicializar todas las transiciones a -1 (estado de error)
    for(int a=0; a<sizeof(automatas)/sizeof(DFA); a++){
        for(int i=0; i<STATES; i++){
            for(int j=0; j<NUM_SYMBOLS; j++){
                automatas[a].transitions[i][j] = -1;
            }
        }
    }

    //Transitions CAT_NUMBER:
    for(int i=0; i<sizeof(automatas[CAT_NUMBER].states)/sizeof(int); i++){
        for(int j=0; j<sizeof(automatas[CAT_NUMBER].alphabet)/sizeof(char); j++){
            automatas[CAT_NUMBER].transitions[i][automatas[CAT_NUMBER].alphabet[j]] = 1;
        }
    }

    //Transitions CAT_IDENTIFIER:
    char c;
    for(int i=0; i<sizeof(automatas[CAT_IDENTIFIER].alphabet)/sizeof(char); i++){
        c = automatas[CAT_IDENTIFIER].alphabet[i];
        if (c >= '0' && c <= '9'){
            automatas[CAT_IDENTIFIER].transitions[automatas[CAT_IDENTIFIER].states[0]][automatas[CAT_IDENTIFIER].alphabet[i]] = automatas[CAT_IDENTIFIER].states[2];
        }
        else {
            automatas[CAT_IDENTIFIER].transitions[automatas[CAT_IDENTIFIER].states[0]][automatas[CAT_IDENTIFIER].alphabet[i]] = automatas[CAT_IDENTIFIER].states[1];
        }
    }

    for(int i=0; i<sizeof(automatas[CAT_IDENTIFIER].alphabet)/sizeof(char); i++){
        automatas[CAT_IDENTIFIER].transitions[automatas[CAT_IDENTIFIER].states[1]][automatas[CAT_IDENTIFIER].alphabet[i]] = automatas[CAT_IDENTIFIER].states[1];
    }

    for(int i=0; i<sizeof(automatas[CAT_IDENTIFIER].alphabet)/sizeof(char); i++){
        automatas[CAT_IDENTIFIER].transitions[automatas[CAT_IDENTIFIER].states[2]][automatas[CAT_IDENTIFIER].alphabet[i]] = automatas[CAT_IDENTIFIER].states[2];
    }

    //Transitions CAT_KEYWORD:
    automatas[CAT_KEYWORD].transitions[0]['i'] = automatas[CAT_KEYWORD].states[1];
    automatas[CAT_KEYWORD].transitions[0]['c'] = automatas[CAT_KEYWORD].states[5];
    automatas[CAT_KEYWORD].transitions[0]['v'] = automatas[CAT_KEYWORD].states[9];
    automatas[CAT_KEYWORD].transitions[0]['e'] = automatas[CAT_KEYWORD].states[13];
    automatas[CAT_KEYWORD].transitions[0]['w'] = automatas[CAT_KEYWORD].states[17];
    automatas[CAT_KEYWORD].transitions[0]['r'] = automatas[CAT_KEYWORD].states[22];
    automatas[CAT_KEYWORD].transitions[1]['f'] = automatas[CAT_KEYWORD].states[2];
    automatas[CAT_KEYWORD].transitions[1]['n'] = automatas[CAT_KEYWORD].states[3];
    automatas[CAT_KEYWORD].transitions[3]['t'] = automatas[CAT_KEYWORD].states[4];
    automatas[CAT_KEYWORD].transitions[5]['h'] = automatas[CAT_KEYWORD].states[6];
    automatas[CAT_KEYWORD].transitions[6]['a'] = automatas[CAT_KEYWORD].states[7];
    automatas[CAT_KEYWORD].transitions[7]['r'] = automatas[CAT_KEYWORD].states[8];
    automatas[CAT_KEYWORD].transitions[9]['o'] = automatas[CAT_KEYWORD].states[10];
    automatas[CAT_KEYWORD].transitions[10]['i'] = automatas[CAT_KEYWORD].states[11];
    automatas[CAT_KEYWORD].transitions[11]['d'] = automatas[CAT_KEYWORD].states[12];
    automatas[CAT_KEYWORD].transitions[13]['l'] = automatas[CAT_KEYWORD].states[14];
    automatas[CAT_KEYWORD].transitions[14]['s'] = automatas[CAT_KEYWORD].states[15];
    automatas[CAT_KEYWORD].transitions[9]['e'] = automatas[CAT_KEYWORD].states[16];
    automatas[CAT_KEYWORD].transitions[17]['h'] = automatas[CAT_KEYWORD].states[18];
    automatas[CAT_KEYWORD].transitions[18]['i'] = automatas[CAT_KEYWORD].states[19];
    automatas[CAT_KEYWORD].transitions[19]['l'] = automatas[CAT_KEYWORD].states[20];
    automatas[CAT_KEYWORD].transitions[20]['e'] = automatas[CAT_KEYWORD].states[21];
    automatas[CAT_KEYWORD].transitions[22]['e'] = automatas[CAT_KEYWORD].states[23];
    automatas[CAT_KEYWORD].transitions[23]['t'] = automatas[CAT_KEYWORD].states[24];
    automatas[CAT_KEYWORD].transitions[24]['u'] = automatas[CAT_KEYWORD].states[25];
    automatas[CAT_KEYWORD].transitions[25]['r'] = automatas[CAT_KEYWORD].states[26];
    automatas[CAT_KEYWORD].transitions[26]['n'] = automatas[CAT_KEYWORD].states[27];
    for(int i=0; i<sizeof(automatas[CAT_KEYWORD].states)/sizeof(int); i++){
        for(int j=0; j<sizeof(automatas[CAT_KEYWORD].alphabet)/sizeof(char); j++){
            if(automatas[CAT_KEYWORD].transitions[i][automatas[CAT_KEYWORD].alphabet[j]] == -1){
                automatas[CAT_KEYWORD].transitions[i][automatas[CAT_KEYWORD].alphabet[j]] = automatas[CAT_KEYWORD].states[28];
            }
        }
    }

    //Transitions CAT_LITERAL:
    for(int i=0; i<sizeof(automatas[CAT_LITERAL].alphabet)/sizeof(char); i++){
        if(automatas[CAT_LITERAL].alphabet[i] == '"'){
            automatas[CAT_LITERAL].transitions[0][automatas[CAT_LITERAL].alphabet[i]] = automatas[CAT_LITERAL].states[1];
        }
        else{
            automatas[CAT_LITERAL].transitions[0][automatas[CAT_LITERAL].alphabet[i]] = automatas[CAT_LITERAL].states[3];
        }
    }

    for(int i=0; i<sizeof(automatas[CAT_LITERAL].alphabet)/sizeof(char); i++){
        if(automatas[CAT_LITERAL].alphabet[i] == '"'){
            automatas[CAT_LITERAL].transitions[1][automatas[CAT_LITERAL].alphabet[i]] = automatas[CAT_LITERAL].states[2];
        }
        else{
            automatas[CAT_LITERAL].transitions[1][automatas[CAT_LITERAL].alphabet[i]] = automatas[CAT_LITERAL].states[3];
        }
    }

    for(int i=0; i<sizeof(automatas[CAT_LITERAL].alphabet)/sizeof(char); i++){
        automatas[CAT_LITERAL].transitions[2][automatas[CAT_LITERAL].alphabet[i]] = automatas[CAT_LITERAL].states[3];
    }

    for(int i=0; i<sizeof(automatas[CAT_LITERAL].alphabet)/sizeof(char); i++){
        automatas[CAT_LITERAL].transitions[3][automatas[CAT_LITERAL].alphabet[i]] = automatas[CAT_LITERAL].states[3];
    }

    //Transitions CAT_OPERATOR:
    for(int i=0; i<sizeof(automatas[CAT_OPERATOR].states)/sizeof(int); i++){
        for(int j=0; j<sizeof(automatas[CAT_OPERATOR].alphabet)/sizeof(char); j++){
            if(i != 2){
                automatas[CAT_OPERATOR].transitions[i][automatas[CAT_OPERATOR].alphabet[j]] = automatas[CAT_OPERATOR].states[i+1];
            }
            else{
                automatas[CAT_OPERATOR].transitions[i][automatas[CAT_OPERATOR].alphabet[j]] = automatas[CAT_OPERATOR].states[i];
            }
        }
    }

    //Transitions CAT_SPECIALCHAR:
    automatas[CAT_SPECIALCHAR].transitions[0]['('] = automatas[CAT_SPECIALCHAR].states[1];
    automatas[CAT_SPECIALCHAR].transitions[0]['{'] = automatas[CAT_SPECIALCHAR].states[3];
    automatas[CAT_SPECIALCHAR].transitions[0]['['] = automatas[CAT_SPECIALCHAR].states[5];
    automatas[CAT_SPECIALCHAR].transitions[0][';'] = automatas[CAT_SPECIALCHAR].states[7];
    automatas[CAT_SPECIALCHAR].transitions[0][','] = automatas[CAT_SPECIALCHAR].states[8];
    automatas[CAT_SPECIALCHAR].transitions[1][')'] = automatas[CAT_SPECIALCHAR].states[2];
    automatas[CAT_SPECIALCHAR].transitions[3]['}'] = automatas[CAT_SPECIALCHAR].states[4];
    automatas[CAT_SPECIALCHAR].transitions[5][']'] = automatas[CAT_SPECIALCHAR].states[6];
    for(int i=0; i<sizeof(automatas[CAT_SPECIALCHAR].states)/sizeof(int); i++){
        for(int j=0; j<sizeof(automatas[CAT_SPECIALCHAR].alphabet)/sizeof(char); j++){
            if(automatas[CAT_SPECIALCHAR].transitions[i][automatas[CAT_SPECIALCHAR].alphabet[j]] == -1){
                automatas[CAT_SPECIALCHAR].transitions[i][automatas[CAT_SPECIALCHAR].alphabet[j]] = automatas[CAT_SPECIALCHAR].states[9];
            }
        }
    }
}

TokenCategory automata_category_for(){
    for(int i=0; i<NUM_AUTOMATAS; i++){
        for(int j=0; j<sizeof(automatas[i].accept_states)/sizeof(int); j++){
            if(automatas[i].current_state == automatas[i].accept_states[j] && automatas[i].accept_states[j] != 0 && alive[i]){
                if(i == CAT_IDENTIFIER){ //Caso especial: si es un identificador, revisar si es keyword
                    for(int k=0; k<sizeof(automatas[CAT_KEYWORD].accept_states)/sizeof(int); k++){
                        if(automatas[CAT_KEYWORD].current_state == automatas[CAT_KEYWORD].accept_states[k] && automatas[CAT_KEYWORD].accept_states[k] != 0){
                            i = CAT_KEYWORD;
                        }
                    }
                }
                switch (i)
                {
                case CAT_NUMBER:
                    return CAT_NUMBER;
                    break;
                case CAT_IDENTIFIER:
                    return CAT_IDENTIFIER;
                    break;
                case CAT_KEYWORD:
                    return CAT_KEYWORD;
                    break;
                case CAT_LITERAL:
                    return CAT_LITERAL;
                    break;
                case CAT_OPERATOR:
                    return CAT_OPERATOR;
                    break;
                case CAT_SPECIALCHAR:
                    return CAT_SPECIALCHAR;
                    break;
                
                default:
                    break;
                }
            }
        }
    }
    return CAT_NONRECOGNIZED;
}

void automata_engine_step(char ch, int *any_alive, int *any_accepting, TokenCategory *best_accepting){
    unsigned char uch = (unsigned char)ch; // Convertir a unsigned char para evitar problemas de índice negativo
    for(int i=0; i<NUM_AUTOMATAS; i++){
        if(automatas[i].transitions[automatas[i].current_state][uch] != -1 && alive[i]){
            automatas[i].current_state = automatas[i].transitions[automatas[i].current_state][uch];
            *any_alive = 1;
        }
        else{
            alive[i] = false;
        }
    }

    *any_accepting = automata_is_accepting();
    if(*any_accepting){
        *best_accepting = automata_category_for();
    }
}

bool automata_is_accepting(){
    for(int i=0; i<NUM_AUTOMATAS; i++){
        for(int j=0; j<sizeof(automatas[i].accept_states)/sizeof(int); j++){
            if(automatas[i].current_state == automatas[i].accept_states[j] && automatas[i].accept_states[j] != 0 && alive[i]){
                return true;
            }
        }
    }
    return false;
}