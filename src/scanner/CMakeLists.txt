add_library(scanner scanner_core.c)
target_include_directories(scanner PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../automata_engine
  ${CMAKE_CURRENT_SOURCE_DIR}/../counters
  ${CMAKE_CURRENT_SOURCE_DIR}/../diagnostics
)
target_link_libraries(scanner PUBLIC token_model diagnostics counters automata_engine)
target_compile_definitions(scanner PRIVATE $<$<CONFIG:Debug>:COUNTCONFIG>)
message(STATUS "(${PROJECT_NAME}) scanner configured: links token_model, diagnostics, counters, automata_engine")
