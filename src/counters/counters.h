/*
  Nombre del programa: Módulo de Counters (Interfaz)
  Autor(es): Iván Torres Ros
  Fecha de creación: 10 de febrero de 2026
  Descripción:
  Contadores COUNTCOMP, COUNTIO, COUNTGEN a nivel preprocesador (COUNTCONFIG).
  Solo activos en DEBUG; en RELEASE no se compilan los incrementos.
  COUNTOUT: 1 = salida al fichero de tokens; 0 = fichero aparte .dbgcnt (COUNTFILE).
  Responsabilidades:
  - Counters (count_comp, count_io, count_gen) y CountConfig (enabled, countout, file).
  - Incrementos add_comp/add_io/add_gen (llamar desde código envuelto en #ifdef COUNTCONFIG).
  - counters_report: mensaje con línea, función, incremento, parcial y total.
*/

#ifndef COUNTERS_H
#define COUNTERS_H

#include <stdio.h>

typedef struct Counters {
    long count_comp;
    long count_io;
    long count_gen;
} Counters;

typedef struct CountConfig {
    int enabled;
    int countout;
    const char *file;
} CountConfig;

void counters_init(Counters *c);
void countconfig_init(CountConfig *cfg);

void counters_add_comp(Counters *c, long delta);
void counters_add_io(Counters *c, long delta);
void counters_add_gen(Counters *c, long delta);

void counters_report(FILE *out, int line, const char *func,
    long inc_comp, long inc_io, long inc_gen,
    const Counters *partial, const Counters *total);

#endif
 