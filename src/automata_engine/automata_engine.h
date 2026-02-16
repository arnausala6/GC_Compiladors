#define NUM_SYMBOLS 128
#define STATES 100
#include <stdbool.h>
#define NUM_AUTOMATAS 6

typedef struct {
  int start_state;
  int states[STATES];
  int current_state;
  int accept_states[STATES];
  char alphabet[NUM_SYMBOLS];
  int transitions[STATES][NUM_SYMBOLS];
} DFA;

extern DFA automatas[6];

typedef enum {
  CAT_NUMBER,
  CAT_IDENTIFIER,
  CAT_KEYWORD,
  CAT_LITERAL,
  CAT_OPERATOR,
  CAT_SPECIALCHAR,
  CAT_NONRECOGNIZED
} TokenCategory;

#define TOKEN_CATEGORY_FROM_AUTOMATA_ENGINE 1

void automata_engine_reset();
TokenCategory automata_category_for();
void automata_engine_step(char ch, int *any_alive, int *any_accepting, TokenCategory *best_accepting);
bool automata_is_accepting();