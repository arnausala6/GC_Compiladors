## Descripción general

El contenido del directorio representa el trabajo principal del proyecto, incluyendo todos los módulos implementados y su integración.

El proyecto se centra en:
  - una definición conceptual clara del diseño
  - una arquitectura modular
  - y una separación explícita entre análisis, gestión de estado y transformación del código

## Directivas soportadas (estado actual)

Se soportan las siguientes directivas del preprocesador:
  #define
  #include "file" (includes locales)
  #ifdef
  #endif

Cualquier otra directiva:
  - no provoca error
  - se ignora o se imprime como código normal
  - no interrumpe la ejecución del programa

Estructura del código fuente

El directorio src/ está organizado en módulos independientes, cada uno con una responsabilidad bien definida:

src/
├── guardar_directivas/     # Parseo y representación estructurada de directivas
├── replace_directives/    # Aplicación de directivas del preprocesador
├── macrostoring/          # Almacenamiento dinámico de macros
├── macrosubstitute/       # Sustitución textual de macros
├── delete_comments/       # Eliminación de comentarios
├── module_preprocessor/  # Motor principal del preprocesador


Cada módulo:
  - tiene una responsabilidad única
  - puede desarrollarse y probarse de forma independiente

## Flujo general del preprocesador
  El motor del preprocesador lee el fichero de entrada carácter a carácter.
  
  Según el modo de ejecución, se eliminan o se conservan los comentarios.
  
  Cuando se detecta el carácter # al inicio de una línea:
    el control se delega al módulo replace_directives.
    
  El módulo replace_directives:
    lee la línea completa de la directiva,
    la analiza mediante guardar_directiva_parse_line,
    decide qué acción aplicar según el tipo de directiva.

  Se actualiza el estado global del preprocesador:
    la tabla de macros (Tabla_macros),
    la pila de condicionales (IfStack).

  El código resultante se escribe en el fichero de salida.

  En el caso de #include, el motor del preprocesador se invoca de forma recursiva, permitiendo includes anidados.

## Módulo guardar_directivas

Este módulo se encarga exclusivamente de detectar y analizar directivas del preprocesador.
No ejecuta directivas ni modifica el código de salida.

Responsabilidades:
  Analizar líneas que contienen directivas.
  
  Identificar el tipo de directiva:
    define, include, ifdef, endif u otras.
  
  Extraer la información relevante:
    nombre y valor de macros,
    fichero incluido,
    localización en el código (SrcLoc).

  Devolver una estructura Directiva con la información parseada.

  Gestionar errores de sintaxis mediante la estructura GDError.

Este módulo es independiente del estado global del preprocesador.

## Módulo replace_directives

Aplica las directivas del preprocesador utilizando la información estructurada proporcionada por guardar_directivas.

Responsabilidades:
  Gestionar directivas #define, #include, #ifdef y #endif.
  
  Mantener una pila de estados condicionales (IfStack) para soportar bloques anidados.
  
  Aplicar directivas solo cuando el bloque actual está activo.
  
  Procesar includes locales mediante llamadas recursivas al motor del preprocesador.
  
  Liberar correctamente la memoria asociada a cada directiva procesada.

## Módulo macrostoring

Gestiona la tabla global de macros del preprocesador.

Responsabilidades:
  Almacenar macros de forma dinámica.
  Comprobar si una macro está definida.
  Proporcionar acceso a la tabla de macros a otros módulos.

## Módulo macrosubstitute

Realiza la sustitución textual de macros durante la lectura del código normal.

Responsabilidades:
  Detectar identificadores en el código.
  Sustituirlos por el cuerpo de la macro si están definidos.
  No altera el flujo de control ni la lógica de directivas.

## Módulo delete_comments

Elimina comentarios del código fuente manteniendo la estructura del programa.

Responsabilidades:
  Eliminar comentarios de línea (//) y de bloque (/* ... */).
  
  Conservar los saltos de línea.

  Mantener la numeración correcta de líneas.

## COMO EJECUTAR EL PROYECTO
Desde la raíz del proyecto, crea una carpeta build, desde la carpeta build haz cmake .. ,  luego haces cmake --build . --config Debug, y se te crea la carpeta Debug.
Desde la carpeta Debug inicias el ejecutable, .\modules_template_main.exe [..\..\tests\(nombre del test).c] [flag]
Aqui tienes la lista de comandos para ejecutar (empiezas en la raiz del proyecto):
mkdir build
cd build
cmake ..
cmake --build . --config Debug
cd Debug
.\modules_template_main.exe [..\..\tests\(nombre del test).c] [flag]


