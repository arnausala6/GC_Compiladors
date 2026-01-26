# -----------------------------------------------------
# src/module_preprocessor/CMakeLists.txt
# Define la librería estática del motor del preprocesador
# -----------------------------------------------------

# Crea la librería estática a partir del motor [cite: 273]
add_library(module_preprocessor preprocessor.c pp_shared.h)

# Permite que otros módulos encuentren los headers de esta carpeta
# Usamos PUBLIC para que el main sepa dónde está "preprocessor.h"
target_include_directories(module_preprocessor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(module_preprocessor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(module_preprocessor PRIVATE module_replacedir delete_comments)

# Si el preprocesador usa funciones de utilidades (como strings manuales), enlaza utils [cite: 273]
# target_link_libraries(module_preprocessor PRIVATE utils)

# Mensaje de confirmación en la consola de CMake [cite: 273]
message(STATUS "(${PROJECT_NAME}) module_preprocessor configured: Added as static library")
