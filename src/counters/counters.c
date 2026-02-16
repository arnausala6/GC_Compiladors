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

void counters_report(FILE *out, int line, const char *func,
    long inc_comp, long inc_io, long inc_gen,
    const Counters *partial, const Counters *total)
{
    if (!out || (!partial && !total)) return;
    long pc = partial ? partial->count_comp : 0;
    long pi = partial ? partial->count_io : 0;
    long pg = partial ? partial->count_gen : 0;
    long tc = total ? total->count_comp : 0;
    long ti = total ? total->count_io : 0;
    long tg = total ? total->count_gen : 0;
    fprintf(out, "line=%d func=%s inc_comp=%ld inc_io=%ld inc_gen=%ld partial_comp=%ld partial_io=%ld partial_gen=%ld total_comp=%ld total_io=%ld total_gen=%ld\n",
            line, func ? func : "?", inc_comp, inc_io, inc_gen, pc, pi, pg, tc, ti, tg);
}
 