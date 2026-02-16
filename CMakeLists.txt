# ------------------------------------------------------------------------------
# Top-level CMakeLists.txt
# CMake configuration for the "modules_template" C project
#
# Purpose:
#   - Create a template with two separate modules to work and test independently
#     with no conflicts when comitting.
#   - Build and run unit tests using CTest (tests/)
#   - Ensure a consistent C11 build environment across platforms
#   - Serve as a GitHub Actions-compatible template for C projects with tests
#
# Structure:
#   - src/    → Contains the codes separated with modules 2 modules (define as many modules as team members)
#   - tests/  → Contains the tests to validate the program in separate parts/modules
#
# This project is meant to be simple, portable, and CI-friendly.
# ------------------------------------------------------------------------------

# CMake top-level build configuration
cmake_minimum_required(VERSION 3.10)

project(modules_template C)
message(STATUS "(${PROJECT_NAME}) Configuring top-level project...")

# Optionally, define a user-friendly name (used in messages, packaging, etc.).
# set(PROJECT_DISPLAY_NAME "modules-template")

# Set language standard
# Set the required C standard to C11: required to use static_assert and other modern features.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
message(STATUS " - (${PROJECT_NAME}) C standard set to C11")

# Specify Debug build type by default (optional)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  message(STATUS " - (${PROJECT_NAME}) Default build type set to Debug")
endif()

# Enable testing support (CTest)
enable_testing()
message(STATUS " - (${PROJECT_NAME}) Testing enabled")

# Add source modules and main program
# add_subdirectory(preprocessor)
# message(STATUS " - (${PROJECT_NAME}) Added preprocessor/ directory")

# Add source modules and main program
add_subdirectory(src)
message(STATUS " - (${PROJECT_NAME}) Added src/ directory")

# Add tests (for each module or overall integration)
add_subdirectory(tests)
message(STATUS " - (${PROJECT_NAME}) Added tests/ directory")

# Optionally build main executable if needed 
# This executable links together all module libraries.
# (to have modular test for each module in parallel)
# add_executable(preprocessor_modules_template_main preprocessor/main.c)
# target_link_libraries(preprocessor_modules_template_main PRIVATE module_preprocessor module_args delete_comments utils module_replacedir guardar_directivas macrostoring macro_substitute)
# message(STATUS " - (${PROJECT_NAME}) Modules Template Main executable 'preprocessor_modules_template_main' linked with module_args and delete_comments and module_preprocessor")
# target_link_libraries(preprocessor_modules_template_main PRIVATE module_preprocessor module_args delete_comments user_manual utils module_replacedir guardar_directivas macrostoring macro_substitute)

# 1. Definimos el ejecutable usando el main.c que está dentro de src/
add_executable(scanner_main src/main.c)

# 2. Enlazamos las librerías necesarias:
#    - scanner_module_args: Para procesar las flags (-i, -trace, etc.)
#    - scanner_utils: Para utils_files.c (manejo de nombres de archivo y timestamps)
target_link_libraries(scanner_main PRIVATE 
    scanner_module_args 
    scanner_utils
    scanner
    output_writer
    token_model
    diagnostics
    counters
    automata_engine
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

message(STATUS " - (${PROJECT_NAME}) Scanner executable 'scanner_main' linked with module_args and utils")