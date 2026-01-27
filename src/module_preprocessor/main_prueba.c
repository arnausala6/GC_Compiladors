#include "./main.h"
#include "./module_preprocessor/preprocessor.h"
#include "./module_replacedir/replace_dir.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <archivo.c>\n", argv[0]);
        return 1;
    }

    // El nombre del archivo de entrada es el último argumento [cite: 37]
    char *nombre_in = argv[argc - 1];
    
    // Abrir el archivo de entrada [cite: 99]
    FILE *in = fopen(nombre_in, "r");
    if (!in) {
        perror("Error al abrir el archivo de entrada");
        return 1;
    }

    // Generar nombre de salida: "input.c" -> "input_pp.c" 
    char nombre_out[256];
    // Lógica simple para renombrar (puedes mejorarla para manejar rutas)
    snprintf(nombre_out, sizeof(nombre_out), "%s_pp.c", nombre_in);

    FILE *out = fopen(nombre_out, "w");
    if (!out) {
        perror("Error al crear el archivo de salida");
        fclose(in);
        return 1;
    }

    printf("Iniciando motor: %s -> %s\n", nombre_in, nombre_out);
    
    // Llamada al motor principal [cite: 52]
    // Por ahora pasamos 0 en flags (comportamiento por defecto) [cite: 26]
    //motor_preprocesador(in, out, 0);

    printf("Proceso finalizado con éxito.\n");

    fclose(in);
    fclose(out);
    return 0;
}