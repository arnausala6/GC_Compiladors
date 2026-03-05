# -----------------------------------------------------------------------------
# language/CMakeLists.txt
#
# Módulo encargado de la carga de la especificación del lenguaje (Gramática,
# Símbolos, y Tablas del Autómata).
# -----------------------------------------------------------------------------

message(STATUS "   - (${PROJECT_NAME}) Configuring 'language' module...")

# 1. Definir la biblioteca estática con sus archivos fuente
add_library(language STATIC language.c)

# 2. Exponer la carpeta actual como un directorio de "include" público.
# Esto permite que otros módulos (como el main o sra_engine) puedan hacer #include "language.h"
target_include_directories(language PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 3. (Opcional) Si tu struct Language incluye el struct SraEngine directamente, 
# el compilador necesitará saber dónde está "sra_engine.h". Descomenta la siguiente línea:
# target_include_directories(language PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../sra_engine)

# 4. Vincular con utilidades si es necesario (según tu CMake principal)
# target_link_libraries(language PUBLIC utils)