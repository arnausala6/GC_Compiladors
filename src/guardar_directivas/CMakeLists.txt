add_library(guardar_directivas
  guardar_directivas.c
  directivas_types.c
)

target_include_directories(guardar_directivas PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(test_guardar_directivas
  guardar_directivas_main.c
)

target_link_libraries(test_guardar_directivas
  guardar_directivas
)
