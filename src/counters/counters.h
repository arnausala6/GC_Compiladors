/*
  Nombre del programa: Módulo de Counters (Interfaz)
 
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 10 de febrero de 2026
 
  Descripción:
  Este archivo define la interfaz del módulo de contadores de operaciones.
  Permite llevar el recuento de comparaciones, E/S y operaciones generales.
 
  Responsabilidades:
  - Definir las estructuras Counters y CountConfig.
  - Declarar funciones para inicializar y actualizar los contadores.
 
  Notas de implementación:
  - Los contadores se almacenan por valor (sin memoria dinámica).
  - La configuración de salida se guarda en CountConfig.
 */

#ifndef COUNTERS_H
#define COUNTERS_H

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

#endif
