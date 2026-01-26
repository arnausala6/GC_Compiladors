# macrostoring/CMakeLists.txt

# Creamos la librería (STATIC o SHARED según convenga)
add_library(macrostoring STATIC macrostoring.c)

# Incluimos el directorio actual para que otros puedan encontrar el header
target_include_directories(macrostoring PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# El propio directorio incluye el .h
target_include_directories(delete_comments PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})