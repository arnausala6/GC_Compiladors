/*
  Nombre del programa: Módulo de Counters (Implementación)
  Autor(es): Iván Torres Ros
  Fecha de creación: 10 de febrero de 2026
  Descripción:
  Implementa incrementos (COUNTCOMP, COUNTIO, COUNTGEN) y mensajes de informe
  (línea, función, incremento, parcial por función, total). El conteo se activa
  solo en DEBUG mediante COUNTCONFIG en los puntos de llamada.
*/

#include "counters.h"
#include <string.h>
#include <stdio.h>

void counters_init(Counters *c) {
    if (!c) return;
    memset(c, 0, sizeof(Counters));
}

void countconfig_init(CountConfig *cfg) {
    if (!cfg) return;
    memset(cfg, 0, sizeof(CountConfig));
    cfg->enabled = 0;
    cfg->countout = 0;
    cfg->file = NULL;
}

void counters_add_comp(Counters *c, long delta) {
    if (c) c->count_comp += delta;
}

void counters_add_io(Counters *c, long delta) {
    if (c) c->count_io += delta;
}

void counters_add_gen(Counters *c, long delta) {
    if (c) c->count_gen += delta;
}

void counters_print(const Counters *c, FILE *out) {
    if (!c) return;
    FILE *stream = out ? out : stdout;
    fprintf(stream, "\n=== CONTADORES DE OPERACIONES ===\n");
    fprintf(stream, "Comparaciones (count_comp): %ld\n", c->count_comp);
    fprintf(stream, "Operaciones E/S (count_io): %ld\n", c->count_io);
    fprintf(stream, "Operaciones generales (count_gen): %ld\n", c->count_gen);
    fprintf(stream, "===================================\n\n");
}
