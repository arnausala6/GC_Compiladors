add_library(automata STATIC automata.c)
target_include_directories(automata PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(automata PUBLIC language)
