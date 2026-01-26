# src/replace_directives/CMakeLists.txt
message(STATUS "(${PROJECT_NAME}) Configuring replace_directives module...")

add_library(replace_directives STATIC
  replace_dir.c
)

target_include_directories(replace_directives PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Dependencias reales del módulo
target_link_libraries(replace_directives
  PUBLIC
    guardar_directivas
    macrostoring
)

message(STATUS "(${PROJECT_NAME}) replace_directives configured: Added as static library")
