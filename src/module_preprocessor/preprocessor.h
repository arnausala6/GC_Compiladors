#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include "main.h"
#include "../macrostoring/macrostoring.h"
#include "./../module_replacedir/replace_dir.h"   
#include "./../delete_comments/delete_comments.h"
#include "./../guardar_directivas/guardar_directivas.h"

typedef enum {
    ESTADO_NORMAL,       // Texto estándar de C
    ESTADO_LINEA_NUEVA,  // Inicio de línea (posible directiva #)
    ESTADO_STRING,       // Dentro de " " 
    ESTADO_CHARACTER,    // Dentro de ' '
    ESTADO_COMENTARIO,   // Detectando /
    ESTADO_MACRO         // Detectando identificador para sustitución 
} EstadoMotor;

// Motor Principal
int motor_preprocesador(FILE *in, FILE *out, int flags, const char *fullpath, Tabla_macros *macros, IfStack *ifstack, GDError *err);

#endif