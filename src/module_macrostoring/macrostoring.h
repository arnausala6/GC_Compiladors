typedef struct {
    char *nombre;
    int num_parametros;
    char **parametros;
    char *cuerpo;
} Macro;

typedef struct {
    int elementos;
    Macro **macros;
}Tabla_macros;

void guardar_macro(Macro tabla_macros, char *nombre, int num_parametros, char **parametros, char *cuerpo);