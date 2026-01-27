# ------------------------------------------------------------------------------
# Programa: P1PP – Preprocesador
# Archivo: CMakeLists.txt (user_manual)
# Autor: Arnau Sala
# Fecha: 20/01/2026
#
# Descripción:
# Configuración CMake del módulo user_manual.
# Define el módulo como una librería estática para ser enlazada con el
# ejecutable principal del preprocesador.
# ------------------------------------------------------------------------------

add_library(user_manual STATIC user_manual.c)
target_include_directories(user_manual PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})