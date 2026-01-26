typedef struct {
    char *nombre;
    char *cuerpo;
} Macro;

typedef struct {
    int elementos;
    Macro **macros;
}Tabla_macros;

void guardar_macro(Tabla_macros tabla_macros, char *nombre, int num_parametros, char **parametros, char *cuerpo);
