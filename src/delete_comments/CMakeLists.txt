# ------------------------------------------------------------------------------
# Programa: P1PP – Preprocesador
# Archivo: CMakeLists.txt (delete_comments)
# Autor: Arnau Sala
# Fecha: 20/01/20026
#
# Descripción:
# Configuración CMake del módulo delete_comments.
# Define el módulo como una librería estática y expone su directorio de
# cabeceras para que pueda ser utilizado por otros módulos del proyecto.
# ------------------------------------------------------------------------------


add_library(delete_comments delete_comments.c)

# El propio directorio incluye el .h
target_include_directories(delete_comments PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Si en el futuro depende de utils u otros módulos:
# target_link_libraries(delete_comments PRIVATE utils)
