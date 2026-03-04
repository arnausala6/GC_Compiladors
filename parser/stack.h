#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

/*
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 4 de marzo de 2026
 
  Descripción:
  Este archivo define la interfaz pública del módulo de pila (`stack`) usado
  por el autómata de análisis sintáctico bottom‑up (Shift‑Reduce Automaton).
  Cada elemento de la pila almacena el estado LR actual y el símbolo de
  gramática asociado (terminal o no terminal).
 
  El módulo expone una pila de capacidad fija, orientada a su uso dentro del
  parser LR:
  - Inicialización de la pila.
  - Inserción de pares (estado, símbolo) en la cima.
  - Extracción y consulta del elemento superior.
  - Comprobación de si la pila está vacía.
 
  Condiciones de uso:
  - La pila tiene un tamaño máximo `STACK_MAX`; un overflow o underflow se
    consideran errores fatales y terminan la ejecución del programa.
  - El módulo no realiza gestión de memoria dinámica ni conoce detalles del
    lenguaje; únicamente almacena enteros (estado y símbolo).
*/

#ifndef STACK_MAX
#define STACK_MAX 1024
#endif

typedef struct {
    int state; 
    int symbol; 
} StackItem;

typedef struct {
    StackItem data[STACK_MAX];
    int top;  
} Stack;

void stack_init(Stack *stack);

void stack_push(Stack *stack, int state, int symbol);

StackItem stack_pop(Stack *stack);

StackItem stack_peek(const Stack *stack);

int stack_is_empty(const Stack *stack);

#endif

