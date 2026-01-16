#include "replace_dir.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *buf;       // memoria del output
  int len;         // bytes usados (sin contar '\0')
  int cap;         // capacidad total
  int max_bytes;   // 0 = sin limite; si >0, limite de salida
} StrBuf;

static int sb_init(StrBuf *sb, int initial_cap, int max_bytes) {
  if (!sb) return 1;

  sb->len = 0;
  sb->cap = (initial_cap > 0) ? initial_cap : 1024;
  sb->max_bytes = max_bytes;

  sb->buf = (char *)malloc((size_t)sb->cap);
  if (!sb->buf) return 1;

  sb->buf[0] = '\0';
  return 0;
}

static void sb_free(StrBuf *sb) {
  if (!sb) return;
  free(sb->buf);
  sb->buf = NULL;
  sb->len = 0;
  sb->cap = 0;
  sb->max_bytes = 0;
}

static int sb_reserve(StrBuf *sb, int extra) {
  if (!sb || extra < 0) return 1;

  // si hay limite de tamaño, comprobarlo
  if (sb->max_bytes > 0) {
    if (sb->len + extra > sb->max_bytes) return 1;
  }

  int need = sb->len + extra + 1; // +1 para '\0'
  if (need <= sb->cap) return 0;

  int new_cap = sb->cap;
  while (new_cap < need) {
    new_cap *= 2;
    if (new_cap <= 0) return 1; // overflow
  }

  char *p = (char *)realloc(sb->buf, (size_t)new_cap);
  if (!p) return 1;

  sb->buf = p;
  sb->cap = new_cap;
  return 0;
}

static int sb_append_n(StrBuf *sb, const char *s, int n) {
  if (!sb || !s || n <= 0) return 0;

  if (sb_reserve(sb, n) != 0) return 1;

  memcpy(sb->buf + sb->len, s, (size_t)n);
  sb->len += n;
  sb->buf[sb->len] = '\0';
  return 0;
}

static int sb_append_c(StrBuf *sb, char c) {
  if (!sb) return 1;

  if (sb_reserve(sb, 1) != 0) return 1;

  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
  return 0;
}

/* =========================
 * Gestion de Errores
 * ========================= */

static void ds_set_error(DSError *err, SrcLoc loc, const char *msg) {
  if (!err) return;
  err->loc = loc;
  snprintf(err->msg, sizeof(err->msg), "%s", (msg ? msg : "Unknown error"));
}

/* =========================
 * IfStack
 * ========================= */

#define DS_MAX_IF_DEPTH 64

typedef struct {
  int active[DS_MAX_IF_DEPTH]; // 1 = copiar, 0 = ignorar
  int depth;                  // nivel actual
} IfStack;

// Inicializamos: nivel 0 siempre activo
static void ifs_init(IfStack *st) {
  st->depth = 0;
  st->active[0] = 1;
}

// Entrar en un nuevo #ifdef / #ifndef
static int ifs_push(IfStack *st, int cond_true) {
  if (!st) return 1;
  if (st->depth >= DS_MAX_IF_DEPTH - 1) return 1; // overflow

  int parent_active = st->active[st->depth];
  st->depth++;

  // Solo estamos activos si el padre lo estaba Y la condición es true
  st->active[st->depth] = (parent_active && cond_true) ? 1 : 0;
  return 0;
}

// Salir de un #endif
static int ifs_pop(IfStack *st) {
  if (!st) return 1;
  if (st->depth <= 0) return 1; // endif sin ifdef

  st->depth--;
  return 0;
}

// ¿Debemos copiar el código ahora?
static int ifs_is_active(const IfStack *st) {
  return st ? st->active[st->depth] : 0;
}
