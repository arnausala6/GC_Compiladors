add_library(parser_debug STATIC debug.c)
target_include_directories(parser_debug PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parser_debug PUBLIC stack)
