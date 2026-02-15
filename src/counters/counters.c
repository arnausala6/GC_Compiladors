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
