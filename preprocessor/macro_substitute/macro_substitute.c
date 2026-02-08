#include "macro_substitute.h"
#include <string.h>

/*
  Nombre del programa: Módulo de Sustitución de Macros (Implementación)
 
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 24 de enero de 2026
 
  Descripción:
  Este archivo contiene la implementación del módulo encargado de realizar
  la sustitución textual de macros durante el proceso de preprocesado.
 
  El módulo recibe identificadores completos desde el motor del preprocesador
  y comprueba si coinciden con alguna macro almacenada en la tabla de macros.
  En caso afirmativo, escribe directamente en el fichero de salida el cuerpo
  de la macro correspondiente.
 
  Este archivo no gestiona la lectura carácter a carácter del fichero de
  entrada ni la detección de identificadores. Dichas responsabilidades
  pertenecen exclusivamente al preprocesador.
 
  Responsabilidades:
  - Recorrer la tabla de macros ya definida.
  - Comparar identificadores con los nombres de las macros.
  - Realizar la sustitución textual escribiendo en el fichero de salida.
 
  Notas de implementación:
  - La sustitución es puramente textual.
  - Actualmente solo se soportan macros sin parámetros.
  - La función devuelve un valor que indica al preprocesador si la
    sustitución se ha realizado o no, para decidir cómo continuar.
 */
int sustituir_macro(
    const char *ident,
    const Tabla_macros *tabla,
    FILE *output
) {
    if (!ident || !tabla || !output) {
        return 0;
    }

    for (int i = 0; i < tabla->elementos; i++) {
        Macro *m = tabla->macros[i];

        if (strcmp(ident, m->nombre) == 0) {
            // Macro encontrada → escribir su cuerpo
            fputs(m->cuerpo, output);
            return 1;
        }
    }

    // No es una macro → no se sustituye
    return 0;
}