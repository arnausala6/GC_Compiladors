# -----------------------------------------------------
# src/user_manual/CMakeLists.txt
# CMakeLists.txt for user_manual module
# -----------------------------------------------------

add_library(user_manual STATIC user_manual.c)
target_include_directories(user_manual PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})