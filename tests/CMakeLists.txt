# -----------------------------------------------------------------------------
# tests/CMakeLists.txt
#
# This file configures unit tests for each module using CTest. Each test is
# built as a standalone executable and linked with the corresponding module,
# allowing for isolated unit testing and debugging.
# -----------------------------------------------------------------------------
 
message(STATUS "(${PROJECT_NAME}) Configuring test executables for modules...")

# Test for module_args
# add_executable(test_t3 t3.c)
# target_include_directories(test_t3 PRIVATE ${PROJECT_SOURCE_DIR}/src)
# add_test(NAME TestModuleArgs COMMAND test_t3 arg90 arg91 arg92)
# message(STATUS " - (${PROJECT_NAME}) Test for module_args added")

# --- Test para t3.c ---
# Ejecuta scanner_main con t3.c como argumento.
add_test(NAME Test_t3 
         COMMAND scanner_main -i "${CMAKE_CURRENT_SOURCE_DIR}/t3.c"
)
message(STATUS " - (${PROJECT_NAME}) Test t3 added (running scanner_main on t3.c)")

# --- Test Lexer / Token model (flujo scanner -> TokenList) ---
add_executable(test_lexer_token_model test_lexer_token_model.c)
target_include_directories(test_lexer_token_model PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(test_lexer_token_model PRIVATE scanner counters)
add_test(NAME TestLexerTokenModel COMMAND test_lexer_token_model)
message(STATUS " - (${PROJECT_NAME}) Test test_lexer_token_model added (TestLexerTokenModel)")

# --- Test todos los módulos (diagnostics, counters, automata_engine, token_model, scanner, output_writer, module_args) ---
add_executable(test_all_modules test_all_modules.c)
target_include_directories(test_all_modules PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(test_all_modules PRIVATE scanner_module_args scanner output_writer)
add_test(NAME TestAllModules COMMAND test_all_modules)
message(STATUS " - (${PROJECT_NAME}) Test test_all_modules added (TestAllModules)")

# add_executable(test_module_2 test_module_2.c)
# target_link_libraries(test_module_2 PRIVATE module_2 utils)
# target_include_directories(test_module_2 PRIVATE ${PROJECT_SOURCE_DIR}/src)
# add_test(NAME TestModule_2 COMMAND test_module_2)
# message(STATUS " - (${PROJECT_NAME}) Test for module_2 added")

# message(STATUS " - (${PROJECT_NAME}) Test configuration (executables) completed.")


# Test for delete_comments
# add_executable(test_scanner_main t3.c)

# target_link_libraries(test_scanner_main PRIVATE 
#     scanner_module_args 
#     scanner_utils
#     # scanner_module_preprocessor  <-- Descomentar solo si existen
#     # delete_comments              <-- Descomentar solo si existen
# )

# target_include_directories(test_scanner_main PRIVATE ${PROJECT_SOURCE_DIR}/src)

# AQUÍ ESTÁ EL CAMBIO:
# Pasamos la ruta absoluta de t3.c como argumento al ejecutable del test
# add_test(NAME TestDeleteComments 
#          COMMAND test_scanner_main "${CMAKE_CURRENT_SOURCE_DIR}/t3.c"
# )

# message(STATUS " - (${PROJECT_NAME}) Test for delete_comments added")