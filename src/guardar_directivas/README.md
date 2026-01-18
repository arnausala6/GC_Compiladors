## Descripción general

Este proyecto implementa un preprocesador de C.

## Directivas soportadas (estado actual)

En el estado actual del proyecto se soportan las siguientes directivas:

    #define

    #include "file"    

    #ifdef

    #endif

Cualquier otra directiva: no provoca error, se ignora o se trata como código normal, no rompe la ejecución del programa

Estructura del proyecto
src/
 ├── guardar_directivas/     # Detección y almacenamiento de directivas
 ├── macrosubstitute/        # Sustitucion de macros
 ├── module-directiva/       # Sustitucion de directivas
 ├── delete_comments/        # Eliminar comentarios
 └── macrostoring/           # Almacenamiento de macros


Cada módulo tiene una responsabilidad clara y se desarrolla de forma independiente.

## Módulo guardar_directivas

    Lee un fichero de código fuente línea a línea

    Detecta líneas que comienzan por #

    Identifica el tipo de directiva

    Extrae la información básica (nombre, valor, fichero, localización)

    Guarda las directivas en una lista dinámica en memoria

!! No ejecuta las directivas ni modifica el código de salida.
Su única responsabilidad es detectar y almacenar información estructurada para otros módulos.

Compilación

Desde la carpeta del modulo:
mkdir build
cd build
cmake ..
cmake --build .


Ejecución

./test_guardar_directivas ../test.c


Ejemplo básico

Entrada (test.c):

#define X 10
#ifdef X
int a;
#endif


Salida esperada:

La directiva #define y #ifdef se detectan
Se almacenan en memoria


Limitaciones actuales

No se soportan expresiones en #if o #elif
No se procesan includes del sistema (<stdio.h>)
No se genera un fichero de salida final