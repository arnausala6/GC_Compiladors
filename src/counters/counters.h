#ifndef COUNTERS_H
#define COUNTERS_H

#ifdef __cplusplus
extern "C" {
#endif

/* --- Según diapositivas: Counters, CountConfig --- */

typedef struct Counters {
    long count_comp;
    long count_io;
    long count_gen;
} Counters;

/* COUNTCONFIG: enabled. COUNTOUT: countout. file: nombre .dbgcnt */
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

#ifdef __cplusplus
}
#endif

#endif /* COUNTERS_H */
