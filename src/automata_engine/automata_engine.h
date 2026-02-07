#define NUM_SYMBOLS 128
#define STATES 100

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

void automata_initialize();
void automata_reset();

