#ifndef PP_SHARED_H
#define PP_SHARED_H

#ifdef __cplusplus
extern "C" {
#endif

// --- Localización de errores / debug ---
typedef struct {
  const char *file;
  int line;
  int col;
} SrcLoc;

// --- Error genérico usado entre módulos ---
typedef struct {
  SrcLoc loc;
  char msg[256];
} GDError;

// --- Stack de condicionales (#ifdef/#endif) ---
#define DS_MAX_IF_DEPTH 64
typedef struct {
  int active[DS_MAX_IF_DEPTH];
  int depth;
} IfStack;

#ifdef __cplusplus
}
#endif

#endif // PP_SHARED_H
