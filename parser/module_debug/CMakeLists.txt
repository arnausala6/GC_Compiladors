add_library(debug STATIC debug.c)
target_include_directories(debug PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(debug PUBLIC stack)
