add_library(guardar_directivas
  guardar_directivas.c
  directivas_types.c
)

target_include_directories(guardar_directivas PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(guardar_directivas
  module_preprocessor
)
