# src/module_replacedir/CMakeLists.txt
message(STATUS "(${PROJECT_NAME}) Configuring module_replacedir module...")

add_library(module_replacedir STATIC
  replace_dir.c
)

target_include_directories(module_replacedir PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Dependencias reales del módulo
target_link_libraries(module_replacedir
    guardar_directivas
    macrostoring
    module_preprocessor 
)

message(STATUS "(${PROJECT_NAME}) module_replacedir configured: Added as static library")