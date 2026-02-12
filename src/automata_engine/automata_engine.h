#define NUM_SYMBOLS 128
#define STATES 100
#include <stdbool.h>

extern DFA automatas[6];

typedef struct {
  int start_state;
  int states[STATES];
  int current_state;
  int accept_states[STATES];
  char alphabet[NUM_SYMBOLS];
  int transitions[STATES][NUM_SYMBOLS]; 
} DFA;

typedef enum {
  CAT_NUMBER,
  CAT_IDENTIFIER,
  CAT_KEYWORD,
  CAT_LITERAL,
  CAT_OPERATOR,
  CAT_SPECIALCHAR,
  CAT_NONRECOGNIZED
} TokenCategory;

typedef enum{
  DFA_RUNNING = 1,
  DFA_FAIL = -1
} DfaState;

void automata_initialize();
DfaState automata_reset();
TokenCategory automata_category_for();
DfaState automata_step(char ch);
bool automata_is_accepting();
