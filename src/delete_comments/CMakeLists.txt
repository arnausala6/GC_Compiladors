# -----------------------------------------------------
# src/delete_comments/CMakeLists.txt
# CMakeLists.txt for delete_comments module
#
# Define el módulo delete_comments como librería estática.
# -----------------------------------------------------

add_library(delete_comments delete_comments.c)

# El propio directorio incluye el .h
target_include_directories(delete_comments PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Si en el futuro depende de utils u otros módulos:
# target_link_libraries(delete_comments PRIVATE utils)
