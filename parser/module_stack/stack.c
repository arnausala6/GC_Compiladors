#include "stack.h"

/*
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 4 de marzo de 2026
 
  Descripción:
  Este archivo implementa las operaciones del módulo de pila (`stack`) usado
  por el autómata de análisis sintáctico bottom‑up (Shift‑Reduce Automaton).
  Proporciona primitivas para inicializar la pila, apilar y desapilar pares
  (estado, símbolo), consultar el elemento superior y comprobar si la pila
  está vacía.
 
  Condiciones de uso:
  - Se asume que la pila se inicializa con `stack_init` antes de su uso.
  - Si se intenta hacer `push` cuando la pila está llena o `pop/peek` cuando
    está vacía, el módulo informa del error por `stderr` y termina el programa
    con `exit(EXIT_FAILURE)`, ya que se considera una inconsistencia grave en
    el funcionamiento del parser.
*/

void stack_init(Stack *stack){
    if (!stack) return;
    stack->top = 0;
}

int stack_is_empty(const Stack *stack){
    return !stack || stack->top == 0;
}

static void stack_abort(const char *msg){
    fprintf(stderr, "[STACK] %s\n", msg);
    exit(EXIT_FAILURE);
}

void stack_push(Stack *stack, int state, int symbol){
    if (!stack) {
        stack_abort("Puntero nulo en stack_push.");
    }
    if (stack->top >= STACK_MAX) {
        stack_abort("Overflow de pila.");
    }

    stack->data[stack->top].state  = state;
    stack->data[stack->top].symbol = symbol;
    stack->top++;
}

StackItem stack_pop(Stack *stack){
    if (!stack) {
        stack_abort("Puntero nulo en stack_pop.");
    }
    if (stack->top <= 0) {
        stack_abort("Underflow de pila (pop sobre pila vacía).");
    }

    stack->top--;
    return stack->data[stack->top];
}

StackItem stack_peek(const Stack *stack){
    if (!stack) {
        stack_abort("Puntero nulo en stack_peek.");
    }
    if (stack->top <= 0) {
        stack_abort("Underflow de pila (peek sobre pila vacía).");
    }

    return stack->data[stack->top - 1];
}

