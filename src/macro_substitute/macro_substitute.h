#ifndef MACRO_SUBSTITUTE_H
#define MACRO_SUBSTITUTE_H

#include <stdio.h>
#include "macrostoring.h"

/*
  Nombre del programa: Módulo de Sustitución de Macros (Interfaz)
 
  Autor(es): Iván Torres Ros
 
  Fecha de creación: 24 de enero de 2026
 
  Descripción:
  Este archivo define la interfaz pública del módulo de sustitución de macros.
  Proporciona la función que permite al preprocesador comprobar si un
  identificador corresponde a una macro y, en ese caso, aplicar su sustitución.
 
  El módulo asume que el identificador ya ha sido leído y validado por el
  motor del preprocesador. No se encarga de la lectura del fichero ni del
  análisis léxico del código fuente.
 
  Funcionalidad expuesta:
  - Recibir un identificador como cadena de caracteres.
  - Consultar la tabla de macros previamente definida.
  - Indicar si la sustitución se ha realizado o no.
 
  Condiciones de uso:
  - Este módulo solo soporta macros sin parámetros.
  - Depende del módulo macrostoring para acceder a la tabla de macros.
  - El preprocesador es responsable de gestionar el flujo del programa
    en función del valor devuelto por la función.
 */

int sustituir_macro(
    const char *ident,
    const Tabla_macros *tabla,
    FILE *output
);

#endif