/*
  Nombre del programa: Módulo de Counters (Implementación)
 
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 10 de febrero de 2026
 
  Descripción:
  Este archivo implementa las funciones del módulo de contadores. Incluye la
  inicialización de contadores/configuración y las operaciones de incremento.
 
  Responsabilidades:
  - Inicializar Counters a cero.
  - Inicializar CountConfig con valores por defecto.
  - Incrementar los contadores (comparaciones, E/S y generales).
 
  Notas de implementación:
  - La inicialización se hace con memset para asegurar valores a cero.
  - Las funciones add_* suman el delta indicado al contador correspondiente.
 */

#include "counters.h"
#include <string.h>

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
