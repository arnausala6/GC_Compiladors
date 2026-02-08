# macro_substitute/CMakeLists.txt

# Creamos la librería (STATIC o SHARED según convenga)
add_library(macro_substitute STATIC
    macro_substitute.c
)

# Incluimos el directorio actual para que otros puedan encontrar el header
target_include_directories(macro_substitute PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link con otros módulos si lo necesita (ej. macrostoring)
target_link_libraries(macro_substitute PRIVATE macrostoring)